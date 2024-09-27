#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable_list.h"

class bvh_node :public hittable {
    public:
        bvh_node(hittable_list list) : bvh_node(list.objects) {}


        
        bvh_node(std::vector<shared_ptr<hittable>>& objects) {

            if(objects.size() == 1) {
                left = right = objects[0];
            } else if(objects.size() == 2) {
                left = objects[0];
                right = objects[1];
            } else if(objects.size() > 2) {
            
                // find bbox
                bbox = aabb();
                interval centroid_interval[3] = {interval(infinity, -infinity)};
                for(int i = 0; i < objects.size(); i++) {
                    bbox = aabb::aabb_union(bbox, objects[i]->bounding_box());
                    for(int j = 0; j < 3; j++) {
                        centroid_interval[j].min = std::min(centroid_interval[j].min, objects[i]->bounding_box().axis_interval[j].centroid());
                        centroid_interval[j].max = std::max(centroid_interval[j].max, objects[i]->bounding_box().axis_interval[j].centroid());
                    }
                }

                // expand the centroid interval to avoid weird shit
                for(int i = 0; i < 3; i++) {
                    centroid_interval[i].expand(0.001);
                }
                

                // try each axis
                float min_cost = infinity;
                int axis = -1;
                int bucket = -1;
                for(int i = 0; i < 3; i++) {
                    
                    // split into buckets and find total surface area in each one
                    double buckets[num_buckets] = {0};
                    int count[num_buckets] = {0};
                    double total_surface_area = 0;
                    int total_count = 0;
                    for(int j = 0; j < objects.size(); j++) {
                        auto object_bb = objects[j]->bounding_box();
                        auto ind = int((object_bb.axis_interval[i].centroid() - centroid_interval[i].min) / centroid_interval[i].size() * num_buckets);
                        auto surface_area = object_bb.surface_area();
                        buckets[ind] += surface_area;
                        count[ind]++;
                        total_surface_area += surface_area;
                        total_count++;
                    }

                    // find total costs of splitting in between each bucket
                    double left_area = 0;
                    int left_count = 0;
                    double right_area = total_surface_area;
                    int right_count = total_count;
                    for(int j = 0; j < num_buckets -1 ; j++) {
                        left_area += buckets[j];
                        left_count += count[j];
                        right_area -= buckets[j];
                        right_count -= count[j];
                        auto cost = left_area * left_count + right_area * right_count;
                        if(cost < min_cost) {
                            min_cost = cost;
                            axis = i;
                            bucket = j;
                        }
                    }
                }

                // now split on this axis
                auto split = ((bucket + 1.0) / num_buckets) * centroid_interval[axis].size() + centroid_interval[axis].min;
                std::vector<shared_ptr<hittable>> left_objects;
                std::vector<shared_ptr<hittable>> right_objects;
                for(int i = 0; i < objects.size(); i++) {
                    if(objects[i]->bounding_box().axis_interval[axis].centroid() < split) {
                        left_objects.push_back(objects[i]);
                    } else {
                        right_objects.push_back(objects[i]);
                    }
                }

                // could optimize memory by doing in-place
                left = make_shared<bvh_node>(left_objects);
                right = make_shared<bvh_node>(right_objects);
            }

            
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            // missed the bounding box
            if(! bbox.hit(r, ray_t)) {
                return false;
            }

            // check left
            bool hit_left =  left->hit(r,ray_t, rec);
            // check if we hit right before we hit the left
            bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

            return hit_left || hit_right;
        }

        aabb bounding_box() const override {return bbox;}

        const aabb empty = aabb(interval::empty, interval::empty, interval::empty);
        const aabb universe = aabb(interval::universe, interval::universe, interval::universe);

    private:
        const int num_buckets = 12;
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        aabb bbox;

    
};



#endif