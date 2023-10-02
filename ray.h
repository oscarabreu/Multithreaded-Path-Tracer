// Include Guards. Prevents multiple-inclusion.

#ifndef RAY_H
#define RAY_H

// Necessary include! Ray uses point3 and vec3.
#include "vec3.h"

// Ray class. Represents a ray in 3D space.
class ray {
  private:
    point3 orig;
    vec3 dir;
  public:
    ray() {} // Default constructor.

    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {} // Member initialization list constructor. See comment on line 31!

    point3 origin() const  { return orig; } // Getter method for origin.
    vec3 direction() const { return dir; }  // Getter method for direction.

  // This 'at' method computes the point along the ray at a distance t * direction from its origin.
    point3 at(double t) const {
        return orig + t*dir;
    }

};

#endif


    // Learned something new here. 

    // Initializing member variables side-steps the two-step process of 
    // default construction and then assignment. This is more efficient/performant - especially for complex objects!
    // In addition, if a class contains 'const' or reference members, you can't reassign consts post-initialization,
    // and references must be bound. So, you must use initialization lists for both.
    
    // See: https://stackoverflow.com/questions/2785612/c-whats-the-difference-between-initialization-and-assignment