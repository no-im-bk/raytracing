# BK Raytraces in 1 weekend

A repo of all the code I wrote while reading the [Ray Tracing in One Weekend Book Series](https://raytracing.github.io)

## Book 1: Ray Tracing in One Weekend

### Files

| Chapter | File | Description |
| --- | --- | --- |
| 2 | [main.cpp](src/main.cpp) | Creates world to render |
| 3 | [vec3.h](src/vec3.h) | Vector utility class copied from book |
| 3 | [color.h](src/color.h) | Color utility class |
| 4 | [rays.h](src/rays.h) | Ray utility class |
| 6 | [hittable.h](src/hittable.h) | abstract class for objects a ray can hit |
| 6 | [hittable_list.h](src/hittable_list.h) | class to contain a list of hittables that represent the world |
| 6 | [sphere.h](src/sphere.h) | class that extends hittable to represent a sphere |
| 6 | [rtweekend.h](src/rtweekend.h) | main header with utilities |
| 6 | [interval.h](src/interval.h) | class to represent real intervals |
| 7 | [camera.h](src/camera.h) | class to represent the camera |

### Images

| Chapter | File | Description |
| --- | --- | --- |
| 2 | [helloworld.ppm](images/helloworld.ppm) | Creates a gradient square |
| 4 | [rays.ppm](images/rays.ppm) | Uses y coordinate of ray to generate a gradient |
| 5 | [spheres.ppm](images/spheres.ppm) | creates a sphere on top of rays.ppm |
| 6 | [normals.ppm](images/normals.ppm) | renders the normals of a sphere |
| 6 | [multiple_objects.ppm](images/multiple_objects.ppm) | renders two spheres |
| 7 | [multiple_objects_refactor.ppm](images/multiple_objects_refactor.ppm) | multiple_objects but refactored to use camera.h |
| 8 | [antialiasing.ppm](images/antialiasing.ppm) | multiple_objects but with antialiasing |
| 9 | [diffuse1.ppm](images/diffuse1.ppm) | first diffuse material render |
| 9 | [diffuse2.ppm](images/diffuse2.ppm) | diffuse material render with max depth |
| 9 | [diffuse3.ppm](images/diffuse3.ppm) | diffuse material render with shadow acne removed |
| 9 | [diffuse4.ppm](images/diffuse4.ppm) | diffuse material render with Lambertian reflection |
