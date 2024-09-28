#ifndef IMAGE_WRITER
#define IMAGE_WRITER

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "external/stb_image_write.h"
#include "color.h"
#include <vector>

class image_writer {
    public:
        image_writer(int width, int height, int channels) : width(width), height(height), channels(channels), img() {};

        // adds pixel to image
        void append_pixel(color pColor) {
            static const interval intensity(0.000, 0.999);
            // gamma transform
            auto r = linear_to_gamma(pColor.x());
            auto g = linear_to_gamma(pColor.y());
            auto b = linear_to_gamma(pColor.z());


            // convert 0-1 values to colors
            unsigned char rbyte = int( 256 * intensity.clamp(r));
            unsigned char gbyte = int( 256 * intensity.clamp(g));
            unsigned char bbyte = int( 256 * intensity.clamp(b));

            img.push_back(rbyte);
            img.push_back(gbyte);
            img.push_back(bbyte);
        }

        // writes image to file
        void write(const char* filename) {
            stbi_write_png(filename, width, height, channels, &img[0], width*channels);
        }

    private:
        std::vector<char> img;
        int width;
        int height;
        int channels;
};

#endif