#ifndef SPHERE_H
#define SPHERE_H

#include "Drawable.h"
#include "../scene/Vector3.h"
#include "../scene/Ray.h"

class Sphere : public Drawable {
 public:
  Sphere();
  ~Sphere();
  double Intersects(Ray ray);
  Vector3 GetPerpendicular(Ray ray, double d); 
  
  double radius_;
  Vector3 center_;
};

#endif
