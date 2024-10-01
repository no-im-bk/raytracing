#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend.h"
#include "aabb.h"

class material;

class hit_record {
    public:
        point3 p;
        vec3 normal;
        double t;
        bool front_face;
        shared_ptr<material> mat;
        double u;
        double v;

        /**
         * Determines if ray is coming from front and sets normal against it too
         */
        void set_face_normal(const ray& r, const vec3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
        virtual aabb bounding_box() const = 0;
};

class translate : public hittable {
    public:

        translate(const shared_ptr<hittable> object,const vec3& offset) : object(object), offset(offset) {
            bbox = object->bounding_box() + offset;
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            ray new_r = ray(r.origin() - offset, r.direction(), r.time());
            bool is_hit = object->hit(new_r, ray_t, rec);

            if(is_hit) {
                rec.p = rec.p + offset;
            }

            return is_hit;
        }

        aabb bounding_box() const override {
            return bbox;
        }

    private:
    shared_ptr<hittable> object;
    vec3 offset;
    aabb bbox;
};

#endif