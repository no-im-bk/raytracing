#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"




int main() {

    

    

    // ---------------put objects in world-----------------
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5,0.5,0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0),1000,ground_material));

    for(int i = -11; i < 11; i++) {
        for(int j = -11; j < 11; j++) {
            auto choose_mat = random_double();
            point3 center(i + 0.9*random_double(), 0.2, j + 0.9*random_double());
            if((center-point3(4,0.2,0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if(choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if(choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5,1);
                    auto fuzz = random_double(0,0.5);
                    sphere_material = make_shared<metal>(albedo,fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dialectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto glass_material = make_shared<dialectric>(1.5);
    world.add(make_shared<sphere>(point3(0,1,0), 1.0, glass_material));

    auto lambertian_material = make_shared<lambertian>(color(0.4,0.2,0.1));
    world.add(make_shared<sphere>(point3(-4,1,0), 1.0, lambertian_material));

    auto metal_material = make_shared<metal>(color(0.7,0.6,0.5), 0.0);
    world.add(make_shared<sphere>(point3(4,1,0), 1.0, metal_material));



    // ----------------------------------------------------

    camera cam;

    // set image dimensions
    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat = point3(0,0,0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);

}