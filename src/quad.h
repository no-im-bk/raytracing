#ifndef QUAD_H
#define QUAD_H

#include "vec3.h"
#include "hittable.h"

class quad : public hittable {
    public:
        quad(const point3& Q,const vec3& u_vec,const vec3&v_vec, shared_ptr<material> mat ) : Q(Q), u_vec(u_vec), v_vec(v_vec), n(unit_vector(cross(u_vec,v_vec))), D(dot(n,Q)), mat(mat) {
            set_bounding_box();
        }

        virtual void set_bounding_box() {
            bbox = aabb::aabb_union(aabb(Q, u_vec+Q), aabb(Q+v_vec, u_vec+v_vec+Q));
        }

        aabb bounding_box() const override { return bbox; }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            // find intersection
            double t =( D - dot(n, r.origin()) )/ dot(n, r.direction());
            // make sure the ray isnt in plane and that the ray t is valid
            if(std::fabs(dot(n, r.direction())) > 1e-8 && ray_t.contains(t)) {
                
                // find intersection point
                point3 p = r.at(t);

                point3 pp = p - Q;

                // find coords of point in u v basis
                double u = dot(pp, u_vec) / (u_vec.length_squared());
                double v =  dot(pp, v_vec) / (v_vec.length_squared());

                // check if we are in bounds of quad
                if (u < 0 || u > 1 || v < 0 || v>1) {
                    return false;
                }

                rec.set_face_normal(r, n);
                rec.mat = mat;
                rec.t = t;
                rec.p = p;
                rec.u = u;
                rec.v = v;

                return true;
            } else {
                return false;
            }
            
        }


    private:
        // members that model the quad as a point and two vectors
        point3 Q;
        vec3 u_vec, v_vec;
        shared_ptr<material> mat;
        aabb bbox;
        // members for modeling the plane the quad is in as n dot v = D
        vec3 n;
        double D; 
};

#endif