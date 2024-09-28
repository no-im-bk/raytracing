#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "bvh.h"




void bouncing_spheres(const char* filename) {

    // ---------------put objects in world-----------------
    hittable_list world;

    auto ground_material = make_shared<lambertian>(make_shared<checker_texture>(0.33, color(0.2,0.3,0.1), color(0.9,0.9,0.9)));
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
                    world.add(make_shared<sphere>(center, center + vec3(0, random_double(0, 0.5), 0), 0.2, sphere_material));
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

    world = hittable_list(make_shared<bvh_node>(world));

    // ----------------------------------------------------

    camera cam;

    // set image dimensions
    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat = point3(0,0,0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.num_threads = 4;

    cam.render(world, filename);

}

void checkered_spheres(const char* filename) {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world, filename);
}

void earth(const char* filename) {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(0,0,12);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe), filename);
}

int main(int argc, char * argv[]) {
    switch(argv[1][0]) {
        case '1': bouncing_spheres(argv[2]); break;
        case '2': checkered_spheres(argv[2]); break;
        case '3': earth(argv[2]); break;
    }
}