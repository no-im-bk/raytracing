#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

/**
 * Returns color of ray. Currently a stub.
 */
color ray_color(const ray& r, const hittable& world) {
    // check where ray hits object in world
    hit_record rec;
    if(world.hit(r, interval(0, infinity), rec)) {
        return 0.5*(rec.normal + color(1,1,1));
    }

    // otherwise render background
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1 - a) * color(0,0,0) + a * color(0.7, 0.4, 0.1);
}

int main() {

    // find image dimensions
    double aspect_ratio = 16.0/9.0;
    int image_width = 400;

    // get the image height and make sure its at least 1
    int image_height = std::max(1, int(image_width / aspect_ratio));

    // ---------------put objects in world-----------------
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    // ----------------------------------------------------

    // find viewport dimensions from the image dimensions
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (image_width * 1.0 / image_height);

    // camera settings
    double focal_length = 1.0;
    auto camera_center = point3(0,0,0);

    // find vectors in the direction of the viewport dimensions
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // find pixel delta vectors
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // find the location of the upper left pixel
    // start at camera center and move out to viewport then move to upper left of viewport
    auto viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
    // center of pixel is offset half a pixel
    auto pixel00_loc = viewport_upper_left + 0.5 * pixel_delta_u + 0.5 * pixel_delta_v;


    // RENDER

    printf("P3\n%d %d\n255\n", image_width, image_height);

    for(int i = 0; i < image_height; i++) {
        std::clog << "Scanlines remaining: " << (image_height - i) << "\n" << std::flush;
        for(int j = 0; j < image_width; j++) {
            auto pixel_center = pixel00_loc + (j*pixel_delta_u + i*pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pColor = ray_color(r, world);

            write_color(std::cout, pColor);
        }
    }
    std::clog << "Done!\n";

}