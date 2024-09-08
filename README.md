# BK Raytraces in 1 weekend

A repo of all the code I wrote while reading the [Ray Tracing in One Weekend Book Series](https://raytracing.github.io)

## Book 1: Ray Tracing in One Weekend

| Chapter | File | Description |
| --- | --- | --- |
| 2 | [helloworld.cpp](src/helloworld.cpp) | Creates a [gradient square](images/helloworld.ppm) |
| 3 | [vec3.h](src/vec3.h) | Vector utility class copied from book |
| 3 | [color.h](src/color.h) | Color utility class |
| 4 | [rays.cpp](src/rays.cpp) | Uses y coordinate of ray to generate a [gradient](images/rays.ppm) |
| 4 | [rays.h](src/rays.h) | Ray utility class |
| 5 | [spheres.cpp](src/spheres.cpp) | creates a sphere on top of rays.cpp [here](images/spheres.ppm) |
| 6 | [normals.cpp](src/normals.cpp) | renders the normals of a sphere [here](images/normals.ppm) |
| 6 | [hittable.h](src/hittable.h) | abstract class for objects a ray can hit |
| 6 | [hittable_list.h](src/hittable_list.h) | class to contain a list of hittables that represent the world |
| 6 | [sphere.h](src/sphere.h) | class that extends hittable to represent a sphere |
| 6 | [rtweekend.h](src/rtweekend.h) | main header with utilities |
| 6 | [interval.h](src/interval.h) | class to represent real intervals |
| 6 | [multiple_objects.cpp](src/multiple_objects.cpp) | renders two spheres [here](images/multiple_objects.ppm) |
