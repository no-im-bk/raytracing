#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        // stationary sphere
        sphere(const point3& center, double radius, shared_ptr<material> mat) 
            : center(center, vec3(0,0,0)), radius(std::fmax(0,radius)), mat(mat) {
                auto radius_vector = vec3(radius, radius, radius);
                bbox = aabb(center + radius_vector, center - radius_vector);
            }
        // moving sphere
        sphere(const point3& center_start,const point3& center_end, double radius, shared_ptr<material> mat)
            : center(center_start, center_end - center_start), radius(std::fmax(0,radius)), mat(mat) {
                auto radius_vector = vec3(radius, radius, radius);
                auto bbox_start = aabb(center_start + radius_vector, center_start - radius_vector);
                auto bbox_end = aabb(center_end + radius_vector, center_end - radius_vector);
                bbox = aabb::aabb_union(bbox_start, bbox_end);
            }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            // use quadratic formula to find hit locations
            auto curr_center = center.at(r.time());
            auto a = dot(r.direction(),r.direction());
            auto h = -dot(r.direction(), r.origin() - curr_center); // h = b / -2
            auto c = dot(r.origin() - curr_center, r.origin() - curr_center) - radius * radius;
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
            auto normal_at_hit = (rec.p - curr_center) / radius;
            rec.set_face_normal(r, normal_at_hit);
            rec.mat = mat;
            get_sphere_uv(normal_at_hit, rec.u, rec.v);

            return true;
        }

        aabb bounding_box() const override {return bbox;}

    static void get_sphere_uv(const point3& p, double& u, double& v) {
        u = std::atan2(-p.z(), p.x()) / (2 * pi) + 0.5;
        v = std::acos(-p.y()/p.length()) / pi;
    }

    private:
        ray center;
        double radius;
        shared_ptr<material> mat;
        aabb bbox;
};

#endif