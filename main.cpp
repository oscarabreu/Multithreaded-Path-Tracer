// Include necessary files to build the raytracer! 

#include "rtweekend.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main() {

    // Function to generate a random seed using the current time.
    // Without this, every time you run the program, you will get the same image.

    seed_random();  
    
    // Creates an instance of the hittable_list, which will store all objects in the scene.
    hittable_list world;

    // Our ground is represented as a very large lambertian sphere with a radius of 2500.
    // It is colored gray (Can be recolored!) and whose center is at (0,-2500,0).
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-2500,0), 2500, ground_material));

    // Here, we're generating a bunch of random spheres to populate the scene.
    // The outer loop a is for the x-axis, and inner loop b is for the z-axis.
    // The spheres are placed in a grid pattern, with a random offset on both x,z of up to 0.9 units.
    // The y-axis center of each sphere is 0.2 units above the ground to match their radius of 0.2.
    // The spheres are colored randomly, with a 80% chance of being a diffuse sphere, and 
    // 15% chance of being a metal sphere, and 5% chance of being a glass sphere.

    for (int a = -40; a < 40; a++) {
        for (int b = -40; b < 40; b++) {
            auto choose_mat = random_double(); //Generate a double from 0-1
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double()); // Generate a random sphere-center offset by a random double

            // This if statement ensures that the spheres do not overlap with 
            // either of the two large spheres that we generate after these loops.
            if (((center - point3(4, 0.2, 0)).length() > 0.9) && 
                ((center - point3(-4, 0.2, 0)).length() > 0.9)) {
                shared_ptr<material> sphere_material; 

                if (choose_mat < 0.8) { // 80% chance of being a diffuse sphere.
                    // Multiply two random colors to get a darker color - multiplying two colors with values between 0-1 
                    // will always result in a lower value! This is more visually pleasng. Too bright of colors are not.
                    auto albedo = color::random() * color::random(); 
                    sphere_material = make_shared<lambertian>(albedo); // Create a lambertian material with the random color.
                    world.add(make_shared<sphere>(center, 0.2, sphere_material)); // Add the sphere to the world.
                } else if (choose_mat < 0.95) {// 15% chance of being a metal sphere.
                    auto albedo = color::random(0.5, 1); // Random color between 0.5 and 1.
                    auto fuzz = random_double(0, 0.5); // Random fuzziness between 0 and 0.5
                    sphere_material = make_shared<metal>(albedo, fuzz); // Create a metal material with the random color.
                    world.add(make_shared<sphere>(center, 0.2, sphere_material)); // Add the sphere to the world.
                } else { // 5% chance of being a glass sphere.
                    sphere_material = make_shared<dielectric>(1.5); // Create a glass material.
                    world.add(make_shared<sphere>(center, 0.2, sphere_material)); // Add the sphere to the world.
                }
            }
        }
    }

    // Add large sphere made of glass at the left-center of the scene
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material1));

    // Add large sphere made of metal at the right-center of the scene
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    // Initialize cam
    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0; //Defines the dimensions of our image.
    cam.image_width       = 500; // Width of the image in pixels.
    cam.samples_per_pixel = 10; // Number of samples to take per pixel - rays per pixel.
    cam.max_depth         = 50; // Maximum number of bounces for a ray.

    cam.vfov     = 40; // Vertical field-of-view in degrees.
    cam.lookfrom = point3(13,2,3); // Camera origin.
    cam.lookat   = point3(0,0,0); // Point camera is looking at.
    cam.vup      = vec3(0,1,0); // Vector defining the up direction of the camera.

    cam.defocus_angle = 0.6; // Angle of the camera's defocus blur.
    cam.focus_dist    = 10.0; // Distance from the camera to the focal plane.

    cam.render(world); // Render the scene!
}

