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
        // must always return the earliest possible hit by the ray
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

class rotate_y : public hittable {
    public:
        rotate_y(const shared_ptr<hittable> object, double angle) : object(object), cos_angle(std::cos(angle*pi/180)), sin_angle(std::sin(angle*pi/180)) {

            bbox = object->bounding_box();

            point3 min( infinity,  infinity,  infinity);
            point3 max(-infinity, -infinity, -infinity);

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++) {
                        auto x = i*bbox.axis_interval[0].max + (1-i)*bbox.axis_interval[0].min;
                        auto y = j*bbox.axis_interval[1].max + (1-j)*bbox.axis_interval[1].min;
                        auto z = k*bbox.axis_interval[2].max + (1-k)*bbox.axis_interval[2].min;

                        auto newx =  cos_angle*x + sin_angle*z;
                        auto newz = -sin_angle*x + cos_angle*z;

                        vec3 tester(newx, y, newz);

                        for (int c = 0; c < 3; c++) {
                            min[c] = std::fmin(min[c], tester[c]);
                            max[c] = std::fmax(max[c], tester[c]);
                        }
                    }
                }
            }

            bbox = aabb(min, max);
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {

            point3 new_origin = point3(r.origin().x()*cos_angle - r.origin().z()*sin_angle,r.origin().y(),r.origin().x()*sin_angle + r.origin().z()*cos_angle );
            point3 new_direction = point3(r.direction().x()*cos_angle - r.direction().z()*sin_angle,r.direction().y(),r.direction().x()*sin_angle + r.direction().z()*cos_angle );
            ray new_r = ray(new_origin, new_direction, r.time());



            bool is_hit = object->hit(new_r, ray_t, rec);

            if(is_hit) {
                rec.p = point3(rec.p.x()*cos_angle + rec.p.z()*sin_angle,rec.p.y(),-rec.p.x()*sin_angle + rec.p.z()*cos_angle );
                rec.normal = point3(rec.normal.x()*cos_angle + rec.normal.z()*sin_angle,rec.normal.y(),-rec.normal.x()*sin_angle + rec.normal.z()*cos_angle );
                
            }

            return is_hit;
        }

        aabb bounding_box() const override { return bbox; }

    private:
        shared_ptr<hittable> object;
        double sin_angle;
        double cos_angle;
        aabb bbox;
};

#endif