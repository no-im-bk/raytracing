#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

/**
 * Converts a double from degrees to radians
 */
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

/**
 * Generates a random double in [0,1)
 */
inline double random_double() {
    return std::rand() / (RAND_MAX + 1.0);
}

/**
 * Generates a random double between min and max
 */
inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

/**
 * Generates a random int on [min, max)
 */
inline int random_int(int min, int max) {
    return int(random_double(min, max));
}

#include "vec3.h"
#include "interval.h"
#include "ray.h"
#include "color.h"

#endif