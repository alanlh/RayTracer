Alan Hu
NetId: alanlh2
UIUC CS 296-25

This is my work for MP1. All of the features should be included, although it is most definitely a work in progress. The perspective rendering is in the "helloworld.png" file, and the orthographic rendering is in the "hello_ortho.png" file. It should compile and create both a perspective and orthographic image, implement anti-aliasing and shading, and draw spheres, triangles, and planes. The shading doesn't exactly follow the Phong reflection guidelines, however. I used HSLA pixels, and reused the CS225 code for modifying PNGs. Other than that, I don't think I "copied" any code. I mainly referenced scratchapixel for the ray-intersection formulae, and consulted some other resources for general help. 

To run this program, type "make", and "./render". The output should be in the "helloworld.png" and "hello_ortho.png" files. If you wish to thoroughly look through the code, the most important files are main.cpp, Scene.cpp, and Scene.h. 

I really enjoyed working on this. It was exciting seeing the images appear and knowing that I wrote the code to create them. Thank you for teaching this class and helping us with this project! I am definitely interested in taking CS 418 in future semesters. 

Notes/Bugs:
For some reaons, I like anticipating features beyond the basics. So a lot of the code in this is probably unnecessary and convoluted, but my goal was to create something that I could easily build upon later on. As such, it's (very) messy, and there are plenty of commented out lines of code. 

My ColorMixer (shading) formulae is very ugly, partially due to the fact I'm not sure about the relationship between light intensity and color. Ideally, I wanted something that could also account for colored light, and I wanted to distinguish between the intensity of color and it's luminosity, which in HSL, is part of the color. I suppose I could have kept it simple, but where's the fun in that? 

Bug: Triangle and Plane coordinates must be (counter?)clockwise, possibly depending on the ray direction vector. Otherwise, the object won't show. There's probably a straightforward fix, but it'll take me some time to find it. 
