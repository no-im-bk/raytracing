#include "rtweekend.h"
#ifndef PERLIN_H
#define PERLIN_H

class perlin {
    public:
        perlin() {
            for(int i = 0; i < point_count; i++) {
                randvec[i] = unit_vector(vec3::random(-1,1));
            }

            perlin_generate_perm(perm_x);
            perlin_generate_perm(perm_y);
            perlin_generate_perm(perm_z);
        }

        double noise(const point3& p) const {
            auto x = int(std::floor(p.x()));
            auto y = int(std::floor(p.y()));
            auto z = int(std::floor(p.z()));

            double x_fraction = p.x() - std::floor(p.x());
            double y_fraction = p.y() - std::floor(p.y());
            double z_fraction = p.z() - std::floor(p.z());

            double xx = x_fraction*x_fraction*(-2*x_fraction + 3);
            double yy = y_fraction*y_fraction*(-2*y_fraction + 3);
            double zz = z_fraction*z_fraction*(-2*z_fraction + 3);

            double value = 0;

            value += dot(randvec[perm_x[ x    & 255] ^ perm_y[ y    & 255] ^ perm_z[ z    & 255]], vec3(x_fraction  ,y_fraction  ,z_fraction  )) * (1-xx) * (1-yy) * (1-zz);
            value += dot(randvec[perm_x[ x    & 255] ^ perm_y[ y    & 255] ^ perm_z[(z+1) & 255]], vec3(x_fraction  ,y_fraction  ,z_fraction-1)) * (1-xx) * (1-yy) *    zz ;
            value += dot(randvec[perm_x[ x    & 255] ^ perm_y[(y+1) & 255] ^ perm_z[ z    & 255]], vec3(x_fraction  ,y_fraction-1,z_fraction  )) * (1-xx) *    yy  * (1-zz);
            value += dot(randvec[perm_x[ x    & 255] ^ perm_y[(y+1) & 255] ^ perm_z[(z+1) & 255]], vec3(x_fraction  ,y_fraction-1,z_fraction-1)) * (1-xx) *    yy  *    zz ;
            value += dot(randvec[perm_x[(x+1) & 255] ^ perm_y[ y    & 255] ^ perm_z[ z    & 255]], vec3(x_fraction-1,y_fraction  ,z_fraction  )) *    xx  * (1-yy) * (1-zz);
            value += dot(randvec[perm_x[(x+1) & 255] ^ perm_y[ y    & 255] ^ perm_z[(z+1) & 255]], vec3(x_fraction-1,y_fraction  ,z_fraction-1)) *    xx  * (1-yy) *    zz ;
            value += dot(randvec[perm_x[(x+1) & 255] ^ perm_y[(y+1) & 255] ^ perm_z[ z    & 255]], vec3(x_fraction-1,y_fraction-1,z_fraction  )) *    xx  *    yy  * (1-zz);
            value += dot(randvec[perm_x[(x+1) & 255] ^ perm_y[(y+1) & 255] ^ perm_z[(z+1) & 255]], vec3(x_fraction-1,y_fraction-1,z_fraction-1)) *    xx  *    yy  *    zz ;

            return value;
        }

        double turb(const point3& p, int depth) const {
            double total = 0;
            auto pp = p;
            double weight = 1;

            for(int i = 0; i < depth; i++) {
                total += weight * noise(pp);
                weight /= 2;
                pp *= 2;
            }

            return std::fabs(total);
        }

    private:
        static const int point_count = 256;
        vec3 randvec[point_count];
        int perm_x[point_count];
        int perm_y[point_count];
        int perm_z[point_count];

        static void perlin_generate_perm(int* p) {
            for(int i = 0; i < point_count; i++) {
                p[i] = i;
            }

            permute(p, point_count);
        }

        static void permute(int* p, int n) {
            for(int i = n - 1; i > 0; i--) {
                int target = random_int(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }
};

#endif