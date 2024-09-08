#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

        bool hit(const ray& r, interval(ray_t), hit_record& rec) const override{
            // use quadratic formula to find hit locations
            auto a = dot(r.direction(),r.direction());
            auto h = -dot(r.direction(), r.origin() - center); // h = b / -2
            auto c = dot(r.origin() - center, r.origin() - center) - radius * radius;
            auto disciminant = h*h - a * c;
            if(disciminant < 0.0) {
                return false;
            }


            auto sqrt_discrim = std::sqrt(disciminant);
            // find closest hit within acceptable range
            auto root = (h - sqrt_discrim) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + sqrt_discrim) / a;
            }
            if (!ray_t.surrounds(root)) {
                return false;
            }

            // set the record for the hit
            rec.t = root;
            rec.p = r.at(rec.t);
            rec.set_face_normal(r, (rec.p - center) / radius);

            return true;
        }

    private:
        point3 center;
        double radius;
};

#endif