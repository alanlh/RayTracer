#include "Plane.h"
#include "Drawable.h"

#include <math.h>
#include <iostream>

#include "../scene/Vector3.h"
#include "../scene/Ray.h"

Plane::Plane() {
  a_ = Vector3(1, 0, 0);
  b_ = Vector3(0, 1, 0);
  c_ = Vector3(0, 0, 1);
}

Plane::~Plane() {

}

double Plane::Intersects(Ray ray) {
  if (dotProduct(ray.direction, crossProduct(b_ - a_, c_ - a_)) < 0.0001) {
    // If ray is parallel to plane, then will never intersect
    return -1;
  }
  Vector3 n = GetPerpendicular(ray, 0);
  double distance = -1 * (dotProduct(n, ray.source - a_))
    / (dotProduct(n, ray.direction));
  if (distance < 0) {
    return -1;
  }  
  return distance;
}

Vector3 Plane::GetPerpendicular(Ray ray, double d) {
  d = 0;
  Vector3 normal = crossProduct(b_ - a_, c_ - a_);
  if (dotProduct(ray.direction, normal) > 0) {
   normal = normal;
  }
  return normal;
}
