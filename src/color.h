#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "rtweekend.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream& out, const color& pColor) {
    static const interval intensity(0.000, 0.999);
    // convert 0-1 values to colors
    int r = int( 256 * intensity.clamp(pColor.x()));
    int g = int( 256 * intensity.clamp(pColor.y()));
    int b = int( 256 * intensity.clamp(pColor.z()));



    //print
    out << r << ' ' << g << ' ' << b << '\n';
}

#endif