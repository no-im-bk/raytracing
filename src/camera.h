#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
  public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    void render(const hittable& world) {
        initialize(); 
        // RENDER

        printf("P3\n%d %d\n255\n", image_width, image_height);

        for(int i = 0; i < image_height; i++) {
            std::clog << "Scanlines remaining: " << (image_height - i) << "\n" << std::flush;
            for(int j = 0; j < image_width; j++) {
                color pixel_color(0,0,0);
                for(int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(j,i);
                    pixel_color += ray_color(r,max_depth, world);
                }

                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }
        std::clog << "Done!\n";
    }

  private:
    int    image_height;   // Rendered image height
    point3 camera_center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    double pixel_samples_scale;

    void initialize() {
        // get the image height and make sure its at least 1
        image_height = std::max(1, int(image_width / aspect_ratio));

        // set the scale for each sample of a pixel
        pixel_samples_scale = 1.0/samples_per_pixel;

        // find viewport dimensions from the image dimensions
        double viewport_height = 2.0;
        double viewport_width = viewport_height * (image_width * 1.0 / image_height);

        // camera settings
        double focal_length = 1.0;
        camera_center = point3(0,0,0);

        // find vectors in the direction of the viewport dimensions
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // find pixel delta vectors
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // find the location of the upper left pixel
        // start at camera center and move out to viewport then move to upper left of viewport
        auto viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
        // center of pixel is offset half a pixel
        pixel00_loc = viewport_upper_left + 0.5 * pixel_delta_u + 0.5 * pixel_delta_v;


    
    }

    /**
     * Returns a random ray near pixel x,y.
     */
    ray get_ray(int x, int y) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((x + offset.x()) * pixel_delta_u) + ((y + offset.y())* pixel_delta_v);

        return ray(camera_center, pixel_sample - camera_center);
    }

    /**
     * Returns a random point within +/-.5
     */
    vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }


    /**
     * Returns color of ray. 
     */
    color ray_color(const ray& r, const int depth_remaining, const hittable& world) const {

        // return no light if bounced enough times
        if(depth_remaining <=0) {
            return color(0,0,0);
        }

        // check where ray hits object in world
        hit_record rec;

        if(world.hit(r, interval(0.001, infinity), rec)) {
            vec3 direction = rec.normal + random_unit_vector();
            return 0.5*ray_color(ray(rec.p,direction),depth_remaining - 1, world);
        }

        // otherwise render background
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1 - a) * color(1.0,1.0,1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif