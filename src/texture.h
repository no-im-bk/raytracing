#include "color.h"
#include "rtw_stb_image.h"
#include "perlin.h"
#ifndef TEXTURE_H
#define TEXTURE_H

class texture {
    public:
        virtual ~texture() = default;

        virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {

    public:
        solid_color(const color& albedo) : albedo(albedo) {};

        solid_color(double red, double green, double blue) : solid_color(color(red,green,blue)) {};

        color value(double u, double v, const point3& p) const override {
            return albedo;
        }

    private:
        color albedo;
};

class checker_texture : public texture {
    public:
        checker_texture(double scale, shared_ptr<texture> even_texture, shared_ptr<texture> odd_texture) :
            scale(scale), even_texture(even_texture), odd_texture(odd_texture) {};

        checker_texture(double scale, const color& odd_color, const color& even_color) :
            scale(scale), even_texture(make_shared<solid_color>(odd_color)), odd_texture(make_shared<solid_color>(even_color)) {};

        color value(double u, double v, const point3& p) const override {
            point3 p_scaled = p / scale;
            if( (int(std::floor(p_scaled.x())) + int(std::floor(p_scaled.y())) + int(std::floor(p_scaled.z()))) % 2 == 0) {
                return even_texture->value(u,v,p);
            } else {
                return odd_texture->value(u,v,p);
            }
        }

    private:
        double scale;
        shared_ptr<texture> even_texture;
        shared_ptr<texture> odd_texture;
};

class image_texture : public texture {
    public:
        image_texture(const char* filename) : image(filename) {};


        color value(double u, double v, const point3& p) const override {

            //missing image
            if (image.height() <= 0) return color( 1,0,1);

            auto pixel_collumn = int(u * image.width());
            auto pixel_row = int((1-v) * image.height());

            const unsigned char* data = image.pixel_data(pixel_collumn, pixel_row);

            return color(data[0] / 255.0, data[1] / 255.0, data[2] / 255.0);
        }

    private:
        rtw_image image;
};

class noise_texture : public texture {
    public:
        noise_texture() {}

        color value(double u, double v, const point3& p) const override {
            return color(1,1,1) * noise.noise(p);
        }

    private:
        perlin noise;
};

#endif