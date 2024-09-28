#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

class material {
    public:
        virtual ~material() = default;

        /**
         * Returns if a ray is scattered and what the attenuation was
         */
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
            return false;
        }

        virtual color emitted(double u, double v, const point3& p) const {
            return color(0,0,0);
        }
};

class lambertian : public material {
    public:
        lambertian(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}

        lambertian(shared_ptr<texture> tex) : tex(tex) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 scatter_direction = rec.normal + random_unit_vector();

            if(scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            scattered = ray(rec.p, scatter_direction, r_in.time());
            attenuation = tex->value(rec.u, rec.v, rec.p);
            return true;
        }
        
    private:
        shared_ptr<texture> tex;
};

class metal : public material {
    public:
        metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz:1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal );
            scattered = ray(rec.p, unit_vector(reflected) + fuzz * random_unit_vector(), r_in.time());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
        
    private:
        color albedo;
        double fuzz;
};

class dialectric : public material {
    public:
        dialectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0,1.0,1.0);
            double ri = rec.front_face ? refraction_index : 1.0/refraction_index;
            double cos_theta = dot(-unit_vector(r_in.direction()),rec.normal);

            double sin_theta = std::sqrt(std::fmax(1.0 - cos_theta * cos_theta, 0.0));

            if(sin_theta / ri > 1.0) {
                // total internal reflection
                vec3 reflected = reflect(r_in.direction(), rec.normal);
                scattered = ray(rec.p, reflected, r_in.time());
            } else {
                // reflection and refraction based on reflectance
                if(reflectance(cos_theta, ri) > random_double()) {
                    vec3 reflected = reflect(r_in.direction(), rec.normal);
                    scattered = ray(rec.p, reflected, r_in.time());
                } else {
                    vec3 refracted = refract(unit_vector(r_in.direction()), rec.normal, ri);
                    scattered = ray(rec.p, refracted, r_in.time());
                }
                
            }

            
            return true;
        }
        
    private:
        double refraction_index;

        static double reflectance(double cosine, double refraction_index) {
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0 * r0;
            return r0 + (1 - r0) * std::pow(1-cosine,5);
        }
};

class diffuse_light : public material {
    public:
        diffuse_light(shared_ptr<texture> tex) : tex(tex) {}
        diffuse_light(const color& emit) : tex(make_shared<solid_color>(emit)) {}

        color emitted(double u, double v, const point3& p) const override {
            return tex->value(u, v, p);
        }
        
    private:
        shared_ptr<texture> tex;
};

#endif