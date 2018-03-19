CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -Werror -pedantic
LDFLAGS = -std=c++1y -stdlib=libc++ -lpng -lc++abi -lpthread

SCENE_EXE = Scene.o Vector3.o
OBJECTS_EXE = Drawable.o Sphere.o Plane.o Triangle.o
IMAGE_EXE = PNG.o HSLAPixel.o lodepng.o ColorMixer.o


render: main.o $(SCENE_EXE) $(OBJECTS_EXE) $(IMAGE_EXE)
	clang++ main.o $(SCENE_EXE) $(OBJECTS_EXE) $(IMAGE_EXE) $(LDFLAGS) -o render

main.o:main.cpp scene/Scene.h image/HSLAPixel.h image/lodepng/lodepng.h image/PNG.h objects/Drawable.h objects/Sphere.h objects/Triangle.h objects/Plane.h scene/Vector3.h image/ColorMixer.h
	clang++ $(CXXFLAGS) main.cpp -c

Scene.o: scene/Scene.cpp scene/Scene.h image/PNG.h image/HSLAPixel.h scene/Ray.h objects/Drawable.h scene/Vector3.h image/ColorMixer.h
	clang++ $(CXXFLAGS) scene/Scene.cpp -c

PNG.o: image/PNG.cpp image/PNG.h image/lodepng/lodepng.h image/RGB_HSL.h
	clang++ $(CXXFLAGS) image/PNG.cpp -c

HSLAPixel.o: image/HSLAPixel.cpp image/HSLAPixel.h
	clang++ $(CXXFLAGS) image/HSLAPixel.cpp -c

lodepng.o: image/lodepng/lodepng.cpp image/lodepng/lodepng.h
	clang++ $(CXXFLAGS) image/lodepng/lodepng.cpp -c

ColorMixer.o: image/ColorMixer.cpp image/ColorMixer.h image/HSLAPixel.h
	clang++ $(CXXFLAGS) image/ColorMixer.cpp -c

Vector3.o: scene/Vector3.cpp scene/Vector3.h
	clang++ $(CXXFLAGS) scene/Vector3.cpp -c

Drawable.o: objects/Drawable.cpp objects/Drawable.h scene/Vector3.h image/HSLAPixel.h
	clang++ $(CXXFLAGS) objects/Drawable.cpp -c

Sphere.o: objects/Sphere.cpp objects/Sphere.h objects/Drawable.h scene/Vector3.h scene/Ray.h
	clang++ $(CXXFLAGS) objects/Sphere.cpp -c

Triangle.o: objects/Triangle.cpp objects/Triangle.h objects/Drawable.h scene/Vector3.h scene/Ray.h
	clang++ $(CXXFLAGS) objects/Triangle.cpp -c

Plane.o: objects/Plane.cpp objects/Plane.h objects/Drawable.h scene/Vector3.h scene/Ray.h
	clang++ $(CXXFLAGS) objects/Plane.cpp -c

clean:
	rm render *.o
