#include "interval.h"
#ifndef AABB_H
#define AABB_H

class aabb {
    public:
        interval axis_interval[3];
        

        aabb() {}

        aabb(const interval& x, const interval& y, const interval& z) {
            axis_interval[0] = x;
            axis_interval[1] = y;
            axis_interval[2] = z;
        };

        // construct a aabb that contains a and b
        aabb(const point3& a, const point3& b) {
            for(int i = 0; i < 3; i++) {
                axis_interval[i] = (a[i] <= b[i]) ? interval(a[i], b[i]) : interval(b[i], a[i]);
            }
        }

        double surface_area() const {
            return 2.0*(axis_interval[0].size() * axis_interval[1].size() + axis_interval[1].size() * axis_interval[2].size() + axis_interval[2].size() * axis_interval[0].size() );
        }

        static aabb aabb_union(const aabb& a,const aabb& b) {
            return aabb(interval::interval_union(a.axis_interval[0], b.axis_interval[0]), interval::interval_union(a.axis_interval[1], b.axis_interval[1]), interval::interval_union(a.axis_interval[2], b.axis_interval[2]));
        }

        bool hit(const ray& r, interval ray_t) const {
            const point3& ray_orig = r.origin();
            const vec3& ray_dir = r.direction();

            // iterate over each axis
            for(int i = 0; i < 3; i++) {
                auto dir_inv = 1.0 / ray_dir[i];

                // find where the ray intersects the bounds of the box
                auto intersection1 = (axis_interval[i].min - ray_orig[i]) * dir_inv;
                auto intersection2 = (axis_interval[i].max - ray_orig[i]) * dir_inv;
                // TODO: should there be padding here for NaNs that pop up in the case where a ray is in one of these planes

                // find the interval on which the ray is inside the box
                auto dir_interval = interval(std::min(intersection1, intersection2),std::max(intersection1, intersection2));
                
                // take the intersection of that interval and the interval we already have
                ray_t.min = std::max(dir_interval.min, ray_t.min);
                ray_t.max = std::min(dir_interval.max, ray_t.max);

                // if the interval is empty, there is no hit
                if( ray_t.max <= ray_t.min) {
                    return false;
                }
            }
            return true;
        }
};

#endif