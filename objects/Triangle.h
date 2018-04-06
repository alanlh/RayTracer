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

  /**
   * Finds the minimum coordinates of each of the 3 points
   */
  virtual Vector3 min();

  /**
   * Finds the maximum coordinates of each of the 3 points
   */
  virtual Vector3 max();

  /**
   * Uses the centroid
   */
  virtual double center(int axis);
  
  Vector3 a_;
  Vector3 b_;
  Vector3 c_;
};

#endif
