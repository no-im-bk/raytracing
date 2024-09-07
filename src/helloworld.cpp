#include "vec3.h"
#include "color.h"

#include <iostream>

int main() {
    int image_width = 256;
    int image_height = 256;

    printf("P3\n%d %d\n255\n", image_width, image_height);

    for(int i = 0; i < image_height; i++) {
        std::clog << "Scanlines remaining: " << (image_height - i) << "\n" << std::flush;
        for(int j = 0; j < image_width; j++) {
            auto pColor = color(j*1.0/image_width, i*1.0/image_height,0);
            write_color(std::cout, pColor);
        }
    }
    std::clog << "Done!\n";

}