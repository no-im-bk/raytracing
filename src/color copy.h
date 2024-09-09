#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "rtweekend.h"

#include <iostream>

using color = vec3;

inline double linear_to_gamma(double linear_component) {
    // if (linear_component > 0) {
    //     return std::sqrt(linear_component);
    // } else {
    //     return 0;
    // }
    return linear_component;
}

void write_color(std::ostream& out, const color& pColor) {
    static const interval intensity(0.000, 0.999);

    // gamma transform
    auto r = linear_to_gamma(pColor.x());
    auto g = linear_to_gamma(pColor.y());
    auto b = linear_to_gamma(pColor.x());



    // convert 0-1 values to colors
    int rbyte = int( 256 * intensity.clamp(r));
    int gbyte = int( 256 * intensity.clamp(g));
    int bbyte = int( 256 * intensity.clamp(b));



    //print
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif