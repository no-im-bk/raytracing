#ifndef CAMERA_H
#define CAMERA_H

#include <thread>
#include <vector>
#include <mutex>

#include "hittable.h"
#include "material.h"
#include "image_writer.h"

class camera {
  public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;
    double vfov = 90; // vertical fov in degrees
    point3 lookfrom = point3(0,0,0); // camera location
    point3 lookat = point3(0,0,-1); // location of focal length
    vec3 vup = vec3(0,1,0); // direction of up (+y) on the camera
    double defocus_angle = 0;
    double focus_dist = 10;
    int num_threads = 4;
    color background;

    void render(const hittable& world, const char* filename) {
        initialize(); 
        // RENDER

        //printf("P3\n%d %d\n255\n", image_width, image_height);

        

        std::vector<std::thread> thread_vector;

        for(int i = 0; i < num_threads; i++) {
            thread_vector.emplace_back(&camera::render_thread, this, &world, i);
        }

        for(int i = 0; i < num_threads; i++) {
            thread_vector[i].join();
        }

        image_writer img = image_writer(image_width, image_height, 3);

        for(int i = 0; i < image_height; i++) {
            for(int j = 0; j < image_width; j++) {
                img.append_pixel(pixel_samples_scale * image_colors[i][j]);
            }
        }

        img.write(filename);

        
        std::clog << "Done!\n";
    }

    

  private:
    int    image_height;   // Rendered image height
    point3 camera_center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    double pixel_samples_scale;
    int samples_per_thread; // Samples per pixel per thread
    vec3 u,v,w; // camera basis vectors
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;
    std::vector<std::vector<color>> image_colors = std::vector<std::vector<color>>();
    std::mutex image_colors_mutex;

    void initialize() {
        // get the image height and make sure its at least 1
        image_height = std::max(1, int(image_width / aspect_ratio));

        // round the number of samples per pixel
        samples_per_thread = samples_per_pixel / num_threads;
        samples_per_pixel = samples_per_thread * num_threads;

        // set the scale for each sample of a pixel
        pixel_samples_scale = 1.0/samples_per_pixel;

        // camera settings
        camera_center = lookfrom;

        // find viewport dimensions from the image dimensions
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2) * focus_dist;
        double viewport_height = 2 * h;
        double viewport_width = viewport_height * (image_width * 1.0 / image_height);

        // find camera basis vectors
        w = unit_vector(lookfrom - lookat); // (z)
        u = unit_vector(cross(vup,w)); // (x)
        v = cross(w,u);       // (y)

        // find vectors in the direction of the viewport dimensions
        auto viewport_u = viewport_width * u;
        auto viewport_v = -viewport_height * v;

        // find pixel delta vectors
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // find the location of the upper left pixel
        // start at camera center and move out to viewport then move to upper left of viewport
        auto viewport_upper_left = camera_center - (focus_dist * w) - viewport_u/2 - viewport_v/2;

        // center of pixel is offset half a pixel
        pixel00_loc = viewport_upper_left + 0.5 * pixel_delta_u + 0.5 * pixel_delta_v;

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;


        // Initialize the 2d vector repesenting the pixel colors
        for(int i = 0; i < image_height; i++) {
            std::vector<color> vec;
            image_colors.push_back(vec);
            for(int j = 0; j < image_width; j++) {
                image_colors[i].push_back(color(0,0,0));
            }
        }

    
    }

    // thread for rendering
    void render_thread(const hittable* world, int threadNum) {
        std::srand(threadNum);
        for(int i = 0; i < image_height; i++) {
            std::clog << "Thread #" << threadNum << " scanlines remaining: " << (image_height - i) << "\n" << std::flush;
            for(int j = 0; j < image_width; j++) {
                color r_color = color(0,0,0);
                for(int sample = 0; sample < samples_per_thread; sample++) {
                    ray r = get_ray(j,i);
                    r_color += ray_color(r,max_depth, *world);                    
                }
                {
                    std::lock_guard<std::mutex> guard(image_colors_mutex);
                    image_colors[i][j] += r_color;
                }
            }
        }
    }

    /**
     * Returns a random ray near pixel x,y.
     */
    ray get_ray(int x, int y) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((x + offset.x()) * pixel_delta_u) + ((y + offset.y())* pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    /**
     * Returns a random point within +/-.5
     */
    vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
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
            ray scattered;
            color attenuation;
            color emitted_color = rec.mat->emitted(rec.u,rec.v,rec.p);
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, depth_remaining - 1, world) + emitted_color;
            }
            return emitted_color;
        }

        // otherwise render background
        return background;
    }
};

#endif