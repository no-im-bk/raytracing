#include "rtweekend.h"
#ifndef PERLIN_H
#define PERLIN_H

class perlin {
    public:
        perlin() {
            for(int i = 0; i < point_count; i++) {
                randfloat[i] = random_double();
            }

            perlin_generate_perm(perm_x);
            perlin_generate_perm(perm_y);
            perlin_generate_perm(perm_z);
        }

        double noise(const point3& p) const {
            auto x = int(std::floor(p.x()));
            auto y = int(std::floor(p.y()));
            auto z = int(std::floor(p.z()));

            auto xx = p.x() - std::floor(p.x());
            auto yy = p.y() - std::floor(p.y());
            auto zz = p.z() - std::floor(p.z());

            double value = 0;

            value += randfloat[perm_x[ x    & 255] ^ perm_y[ y    & 255] ^ perm_z[ z    & 255]] * (1-xx) * (1-yy) * (1-zz);
            value += randfloat[perm_x[ x    & 255] ^ perm_y[ y    & 255] ^ perm_z[(z+1) & 255]] * (1-xx) * (1-yy) *    zz ;
            value += randfloat[perm_x[ x    & 255] ^ perm_y[(y+1) & 255] ^ perm_z[ z    & 255]] * (1-xx) *    yy  * (1-zz);
            value += randfloat[perm_x[ x    & 255] ^ perm_y[(y+1) & 255] ^ perm_z[(z+1) & 255]] * (1-xx) *    yy  *    zz ;
            value += randfloat[perm_x[(x+1) & 255] ^ perm_y[ y    & 255] ^ perm_z[ z    & 255]] *    xx  * (1-yy) * (1-zz);
            value += randfloat[perm_x[(x+1) & 255] ^ perm_y[ y    & 255] ^ perm_z[(z+1) & 255]] *    xx  * (1-yy) *    zz ;
            value += randfloat[perm_x[(x+1) & 255] ^ perm_y[(y+1) & 255] ^ perm_z[ z    & 255]] *    xx  *    yy  * (1-zz);
            value += randfloat[perm_x[(x+1) & 255] ^ perm_y[(y+1) & 255] ^ perm_z[(z+1) & 255]] *    xx  *    yy  *    zz ;

            return value;
        }

    private:
        static const int point_count = 256;
        double randfloat[point_count];
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