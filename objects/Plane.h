#ifndef PLANE_H
#define PLANE_H

#include "Drawable.h"
#include "../scene/Vector3.h"
#include "../scene/Ray.h"

class Plane : public Drawable {
 public:
  Plane();
  ~Plane();
  double Intersects(Ray ray);
  Vector3 GetPerpendicular(Ray ray, double d);
  
  Vector3 a_;
  Vector3 b_;
  Vector3 c_;
};

#endif
