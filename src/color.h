#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream& out, const color& pColor) {
    // convert 0-1 values to colors
    int r = int( 255.999 * pColor.x());
    int g = int( 255.999 * pColor.y());
    int b = int( 255.999 * pColor.z());

    //print
    out << r << ' ' << g << ' ' << b << '\n';
}

#endif