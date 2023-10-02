#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "material.h"


#include <array>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <iostream>
#include <sstream>
#include <queue>

class camera {
  public:
    double aspect_ratio      = 1.0;  
    int    image_width       = 100;  
    int    samples_per_pixel = 10;   
    int    max_depth         = 10;   

    double vfov     = 90;              
    point3 lookfrom = point3(0,0,-1);  
    point3 lookat   = point3(0,0,0);   
    vec3   vup      = vec3(0,1,0);     

    double defocus_angle = 0;  
    double focus_dist = 10;    
    
struct WorkUnit {
    int start_x;
    int end_x;
    int start_y;
    int end_y;
};
void render(const hittable& world) {
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    initialize();
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    const int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::mutex qMtx;
    std::mutex outputMtx;
    const int blockSize = 8;
    std::atomic<int> blocks_completed(0);
    std::vector<std::vector<std::array<char, 32>>> imageBuffer(image_height, std::vector<std::array<char, 32>>(image_width));
    std::queue<WorkUnit> workQueue;
    for (int j = 0; j < image_height; j += blockSize) {
        for (int i = 0; i < image_width; i += blockSize) {
            WorkUnit wu = {i, std::min(i + blockSize, image_width), j, std::min(j + blockSize, image_height)};
            workQueue.push(wu);
        }
    }
    for (int t = 0; t < num_threads; t++) {
        threads.push_back(std::thread([this, &world, &qMtx, &outputMtx, &workQueue, &blocks_completed, &imageBuffer]() {           
             while (true) {
                WorkUnit wu;
                {
                    std::lock_guard<std::mutex> lock(qMtx);
                    if (workQueue.empty()) {
                        break;
                    }
                    wu = workQueue.front();
                    workQueue.pop();
                }
                for (int j = wu.start_y; j < wu.end_y; ++j) {
                    for (int i = wu.start_x; i < wu.end_x; ++i) {
                        color pixel_color(0, 0, 0);
                        for (int sample = 0; sample < samples_per_pixel; ++sample) {
                            ray r = get_ray(i, j);
                            pixel_color += ray_color(r, max_depth, world);
                        }
                        std::ostringstream oss;  // Create a temporary string buffer.
                        write_color(oss, pixel_color, samples_per_pixel); // Write to the buffer.
                        std::string pixelStr = oss.str(); // Retrieve the string from the buffer.
                        std::copy(pixelStr.begin(), pixelStr.end(), imageBuffer[j][i].begin());
                    }
                }
                {
                    std::lock_guard<std::mutex> lock(outputMtx);
                    blocks_completed.fetch_add(1);
                    std::clog << "\rBlocks completed: " << blocks_completed.load() << "/" << (image_width * image_height) / (blockSize * blockSize) << std::flush;
                }
            }
        }));
    }
    for (std::thread& t : threads) {
        t.join();
    }
    for (const std::vector<std::array<char, 32>>& row : imageBuffer) {
        for (const std::array<char, 32>& cell : row) {
            std::cout.write(&cell[0], std::strlen(&cell[0]));
        }
    }
    std::clog << "\rDone.                 \n";
    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::clog << "Rendering time: " << elapsedTime.count() << " milliseconds\n";
}

private:
    int    image_height;   
    point3 center;          
    point3 pixel00_loc;     
    vec3   pixel_delta_u;   
    vec3   pixel_delta_v;   
    vec3   u, v, w;         
    vec3   defocus_disk_u;  
    vec3   defocus_disk_v;  

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        vec3 viewport_u = viewport_width * u;    
        vec3 viewport_v = viewport_height * -v;  

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 pixel_sample_square() const {
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    vec3 pixel_sample_disk(double radius) const {
        auto p = radius * random_in_unit_disk();
        return (p[0] * pixel_delta_u) + (p[1] * pixel_delta_v);
    }

    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        if (depth <= 0)
            return color(0,0,0);

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
};


#endif
