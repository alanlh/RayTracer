#ifndef PLANE_H
#define PLANE_H

#include "Drawable.h"
#include "../scene/Vector3.h"
#include "../scene/Ray.h"

class Plane : public Drawable {
 public:
  Plane();
  ~Plane();
  virtual double Intersects(Ray ray);
  virtual Vector3 GetPerpendicular(Ray ray, double d);

  // Returns very large and very small numbers, because planes extend to inf.
  virtual Vector3 min();
  virtual Vector3 max();

  /**
   * Since there is no real center, use the centroid instead
   */
  virtual double center(int axis);
  
  Vector3 a_;
  Vector3 b_;
  Vector3 c_;
};

#endif
