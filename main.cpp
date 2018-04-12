#include <iostream>
#include <math.h>
#include <cstdlib>

#include "scene/Scene.h"
#include "image/HSLAPixel.h"
#include "image/lodepng/lodepng.h"
#include "image/PNG.h"
#include "objects/Drawable.h"
#include "objects/Sphere.h"
#include "objects/Triangle.h"
#include "objects/Plane.h"
#include "scene/Vector3.h"

void helloworld();
void lotsospheres(int sphere_count);
void simpleobjects();

int main() {
  helloworld();
  return 0;
}

void simpleobjects() {
  Scene *scene = new Scene();

  Scene::AmbientLight *ambient1 = new Scene::AmbientLight();
  ambient1->color = HSLAPixel(0, 0.5, 1);
  ambient1->intensity = 5;
  scene->AddAmbientLight(ambient1);

  Scene::LightSource *light1 = new Scene::LightSource();
  light1->direction = Vector3(1, -1, 3);
  light1->color = HSLAPixel(120, 1, 1);
  light1->intensity = 4;
  scene->AddLightSource(light1);

  Sphere *sphere = new Sphere();
  sphere->center_ = Vector3(0, 0, 4);
  sphere->radius_ = 1;
  sphere->surface_color_.h = 180;
  sphere->surface_color_.s = 1;
  sphere->surface_color_.l = 0.5;
  scene->AddDrawable(sphere);

  Triangle *triangle1 = new Triangle();
  triangle1->a_ = Vector3(2, 0, 3);
  triangle1->b_ = Vector3(0, 6, 8);
  triangle1->c_ = Vector3(0, -6, 8);
  triangle1->surface_color_ = HSLAPixel(270, 0.5, 0.7);
  scene->AddDrawable(triangle1);

  Triangle *triangle2 = new Triangle();
  triangle2->a_ = Vector3(-2, 0, 3);
  triangle2->b_ = Vector3(0, 6, 8);
  triangle2->c_ = Vector3(0, -6, 8);
  triangle2->surface_color_ = HSLAPixel(90, 0.8, 0.4);
  scene->AddDrawable(triangle2);
  
  Vector3 origin(0, 0, -1);
  Vector3 canvas(0, 0, 0);
  scene->SetCamera(origin, 0, canvas, 600);

  PNG * result = scene->Render(400, 400);
  result->writeToFile("simpleobjects.png");
      
  delete scene;
  
  delete result;  
}

void lotsospheres(int sphere_count) {
  srand(time(0));
  Scene *scene = new Scene();

  Scene::AmbientLight *ambient1 = new Scene::AmbientLight();
  ambient1->color = HSLAPixel(60, 0.75, 1);
  ambient1->intensity = 5;
  scene->AddAmbientLight(ambient1);
  
  Scene::LightSource *light1 = new Scene::LightSource();
  light1->direction = Vector3(1, -1, 1);
  light1->color = HSLAPixel(120, 1, 1);
  light1->intensity = 5;
  scene->AddLightSource(light1);
  
  for (int i = 0; i < sphere_count; i ++) {
    int x = rand() % 50 - 25;
    int y = rand() % 50 - 25;
    int z = rand() % 50 + 25;

    Sphere *sphere = new Sphere();
    sphere->center_ = Vector3(x, y, z);
    sphere->radius_ = 1;
    sphere->surface_color_.h = rand() % 360;
    sphere->surface_color_.s = 1;
    sphere->surface_color_.l = 0.5;
    scene->AddDrawable(sphere);
  }
  Vector3 origin(45, 30, -100);
  Vector3 canvas(15, 10, 0);
  scene->SetCamera(origin, 0, canvas, 900);

  PNG * result = scene->Render(600, 600);
  result->writeToFile("spheresfast.png");
      
  delete scene;
  
  delete result;
}

void helloworld() {
  Scene *scene = new Scene();
  Sphere *sphere1 = new Sphere();
  sphere1->center_ = Vector3(6, -1, -1);
  sphere1->radius_ = 1;
  sphere1->surface_color_.h = 30;
  sphere1->surface_color_.s = 1;
  sphere1->surface_color_.l = 0.5;
  scene->AddDrawable(sphere1);

  Sphere *sphere2 = new Sphere();
  sphere2->center_ = Vector3(6, 1, -1);
  sphere2->radius_ = 1;
  sphere2->surface_color_.h = 210;
  sphere2->surface_color_.s = 1;
  sphere2->surface_color_.l = 0.5;
  scene->AddDrawable(sphere2);

  Sphere *sphere3 = new Sphere();
  sphere3->center_ = Vector3(7, 0, 0.5);
  sphere3->radius_ = 1;
  sphere3->surface_color_.h = 240;
  sphere3->surface_color_.s = 0;
  sphere3->surface_color_.l = 0.5;
  scene->AddDrawable(sphere3);

  Sphere *sphere4 = new Sphere();
  sphere4->center_ = Vector3(8, 1, -1);
  sphere4->radius_ = 1;
  sphere4->surface_color_.h = 30;
  sphere4->surface_color_.s = 1;
  sphere4->surface_color_.l = 0.5;
  scene->AddDrawable(sphere4);

  Sphere *sphere5 = new Sphere();
  sphere5->center_ = Vector3(8, -1, -1);
  sphere5->radius_ = 1;
  sphere5->surface_color_.h = 210;
  sphere5->surface_color_.s = 1;
  sphere5->surface_color_.l = 0.5;
  scene->AddDrawable(sphere5);

  Triangle *triangle1 = new Triangle();
  triangle1->a_ = Vector3(10, -2, -2);
  triangle1->b_ = Vector3(10, 2, -2);
  triangle1->c_ = Vector3(10, -2, 2);
  triangle1->surface_color_.h = 0;
  triangle1->surface_color_.s = 0;
  triangle1->surface_color_.l = 0.5;
  scene->AddDrawable(triangle1);

  Triangle *triangle2 = new Triangle();
  triangle2->a_ = Vector3(10, 2, -2);
  triangle2->b_ = Vector3(10, 2, 2);
  triangle2->c_ = Vector3(10, -2, 2);
  triangle2->surface_color_.h = 0;
  triangle2->surface_color_.s = 0;
  triangle2->surface_color_.l = 0.5;
  scene->AddDrawable(triangle2);

  Plane *plane1 = new Plane();
  plane1->a_ = Vector3(4, 0, -2.5);
  plane1->b_ = Vector3(10, 4, -3);
  plane1->c_ = Vector3(10, -4, -3);
  plane1->surface_color_.h = 150;
  plane1->surface_color_.s = 1;
  plane1->surface_color_.l = 0.5;
  scene->AddDrawable(plane1);

  Plane *plane2 = new Plane();
  plane2->a_ = Vector3(4, 0, 2.5);
  plane2->b_ = Vector3(10, -4, 3);
  plane2->c_ = Vector3(10, 4, 3);
  plane2->surface_color_.h = 210;
  plane2->surface_color_.s = 1;
  plane2->surface_color_.l = 0.5;
  scene->AddDrawable(plane2);
  
  Scene::AmbientLight *ambient1 = new Scene::AmbientLight();
  ambient1->color = HSLAPixel(60, 0.75, 1);
  ambient1->intensity = 2;
  scene->AddAmbientLight(ambient1);
  
  Scene::LightSource *light1 = new Scene::LightSource();
  light1->direction = Vector3(1, -1, 1);
  light1->color = HSLAPixel(120, 1, 1);
  light1->intensity = 5;
  scene->AddLightSource(light1);
  
  Scene::LightSource *light2 = new Scene::LightSource();
  light2->direction = Vector3(1, 1, -1);
  light2->color = HSLAPixel(300, 0, 1);
  light2->intensity = 2;
  scene->AddLightSource(light2);
  
  Vector3 origin(0, 7, 1);
  Vector3 canvas(1.5, 5.5, 0.8);
  scene->SetCamera(origin, 0, canvas, 900);
  
  PNG * result = scene->Render(600, 600);
  result->writeToFile("helloworld.png");
      
  delete scene;
  delete result;
}
