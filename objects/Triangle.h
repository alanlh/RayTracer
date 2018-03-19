#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Drawable.h"
#include "../scene/Vector3.h"
#include "../scene/Ray.h"

class Triangle : public Drawable {
 public:
  Triangle();
  ~Triangle();
  double Intersects(Ray ray);
  Vector3 GetPerpendicular(Ray ray, double d);
  
  Vector3 a_;
  Vector3 b_;
  Vector3 c_;
};

#endif
