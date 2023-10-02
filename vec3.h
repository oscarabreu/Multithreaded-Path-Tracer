// Header-Guard to prevent multiple inclusions.
#ifndef VEC3_H
#define VEC3_H

// Necessary includes.
#include "rtweekend.h"
#include <cmath>
#include <iostream>

// Using declarations - to avoid using namespace std.
using std::sqrt;
using std::fabs;

// `vec3` Class: Represents a 3D vector, which can define 3D points and colors.
// Inside the class definition, we have operations involving the vec3 itself and fundamental data types along with classic getters and setters.
// External utility functions are provided for compound operations between multiple vec3 instances and fundamental data types.

class vec3 {
  public:
    double e[3]; // e[0] = x, e[1] = y, e[2] = z

    vec3() : e{0,0,0} {} // Default constructor
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {} // Constructor for 3 doubles.

    // Getters for x, y, z!
    double x() const { return e[0]; } 
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // Operator overloading - useful for vector math! {

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } // Unary Negation Operator.
    double operator[](int i) const { return e[i]; } // Indexing for consts.
    double& operator[](int i) { return e[i]; } // Indexing for mutable objects.

    // Overloading the + operator for two vec3s to add x, y, z components each! 
    vec3& operator+=(const vec3 &v) { 
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    // Overloading the * operator for vec3s to mult x, y, z components by a const t! 
    // Usage: Multiply a vec3 object by a scalar, modifying the original vec3 in the process.
    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    // Overloading the / operator for vec3s to div x, y, z components by a const t! 
    vec3& operator/=(double t) {
        return *this *= 1/t;
    }
    // } End of Operator Overloading (for now)

    // Usage: Determine if spheres intersect, compare distances without expensive sqrt operation, etc.
    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
    
    // Usage: Normalization, distance between two points, scaling, etc.
    double length() const {
        return sqrt(length_squared());
    }

    // Usage: Return true if the vector is close to zero in all dimensions.
    // Reason: Division by very small values may cause overflow! This is a method to handle this edge case.
    bool near_zero() const {
        auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    
    // Usage: Generate a random vector with x, y, z components between 0 and 1.
    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    // Usage: Generate a random vector with x, y, z components between min and max.
    static vec3 random(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
};

// Point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;

// Compound Vector Utility Functions:
// These functions provide operations involving multiple vec3 instances or interactions with fundamental data types.
// While the class definition handles basic and self-referential operations, these utilities offer more advanced
// and external operations that aren't strictly tied to a single vec3 object.

// Output stream overloaded operator. Allows us to print vec3 x, y, z components to the console.
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) { 
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Overloaded addition operator of two vectors.
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Overloaded subtraction operator of two vectors.
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Overloaded multiplication operator of two vectors.
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Another overloaded multiplication operator of a vector and a const.
// Different from previous mult overloaded - as it multiplies a vec3 object 
// by a scalar, BUT returns a new vec3 in the process --- retaining the original.
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

// Interestingly, this references the previous function - handles the reverse order of multiplication. 
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

// Overloaded division operator of a vector and a const. References the overloaded operator* and multiplies the inverse of t.
inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

// Usage: Dot product of two vectors.
inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

// Usage: Cross product of two vectors.
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Usage: Return a unit vector of the input vector.
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// End Intro 

// Chapter 9 [Diffuse Materials Below!]

inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1,1);
        if (p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

// Vector Utility Functions }


#endif