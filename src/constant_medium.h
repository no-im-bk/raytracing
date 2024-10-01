#ifndef CONSTANT_MEDIUM
#define CONSTANT_MEDIUM

#include "hittable.h"
#include "texture.h"
#include "material.h"


class constant_medium : public hittable {
    public:
        constant_medium(shared_ptr<hittable> boundary, double density, shared_ptr<texture> tex):
            boundary(boundary), neg_inv_density(-1.0/density), phase_function(make_shared<isotropic>(tex)) {};

        constant_medium(shared_ptr<hittable> boundary, double density, color albedo):
            boundary(boundary), neg_inv_density(-1.0/density), phase_function(make_shared<isotropic>(albedo)) {};

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record rec1;
            hit_record rec2;
            double delta = 0.0001;

            if(!boundary->hit(r, interval::universe, rec1)) {
                return false;
            }

            if(!boundary->hit(r, interval(rec1.t+ delta, infinity), rec2)) {
                return false;
            }

            rec1.t = std::max(rec1.t, ray_t.min);
            rec2.t = std::min(rec2.t, ray_t.max);

            if(rec2.t <= rec1.t) {
                return false;
            }

            // why do i need this? 
            // i get that this takes care of the case if the ray starts in the boundary
            // but isnt ray_t already gauranteed to always be positive thus already doing this?
            if(rec1.t < 0) {
                rec1.t = 0;
            }

            auto dist_until_collision = neg_inv_density * std::log(random_double());
            auto dist_to_other_side = (rec2.t-rec1.t) * r.direction().length();

            if(dist_until_collision > dist_to_other_side) {
                return false;
            }

            rec.t = rec1.t + dist_until_collision / r.direction().length();
            rec.p = r.at(rec.t);
            rec.normal = vec3(1,0,0); // doesnt matter
            rec.front_face = true; // doesnt matter
            rec.mat = phase_function;
            return true;

        }

        aabb bounding_box() const override { return boundary->bounding_box(); }

    private:
        shared_ptr<hittable> boundary;
        double neg_inv_density;
        shared_ptr<material> phase_function;
};

#endif