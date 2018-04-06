#include "Sphere.h"
#include "Drawable.h"

#include <math.h>
#include <iostream>

#include "../scene/Vector3.h"
#include "../scene/Ray.h"

Sphere::Sphere() : radius_(1) {
  center_.x_ = 0;
  center_.y_ = 0;
  center_.y_ = 0;
}

Sphere::~Sphere() {
}

/**
 * Used method described from:
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
 * Determines whether or not the ray intersects with the sphere
 * If so, returns the minmum value of t at which it intersects (possibly neg)
 * Otherwise, returns -1.
 */
double Sphere::Intersects(Ray ray) {
  Vector3 o = ray.source;
  Vector3 d = ray.direction;

  Vector3 l = (center_ - o);
  // Solving an evil quadratic >:(

  double a = dotProduct(d, d);
  double b = -2 * (dotProduct(l, d));
  double c = dotProduct(l, l) - pow(radius_, 2);  
  
  double discriminant = pow(b, 2) - 4 * a * c;
  if (discriminant < 0) {
    return -1;
  } else if (discriminant == 0) {
    return (-1 * b) / (2 * a);
  }
  double value1 = (-1 * b + sqrt(discriminant)) / (2 * a);
  double value2 = (-1 * b - sqrt(discriminant)) / (2 * a);
  if (value2 >= 0) {
    return value2; 
  }
  return value1;
}

Vector3 Sphere::GetPerpendicular(Ray ray, double d) {
  Vector3 point = ray.point(d);
  return point - center_;
}

Vector3 Sphere::min() {
  return Vector3(center_.x_ - radius_,
		 center_.y_ - radius_,
		 center_.z_ - radius_);
}

Vector3 Sphere::max() {
  return Vector3(center_.x_ + radius_,
		 center_.y_ + radius_,
		 center_.z_ + radius_);
}

double Sphere::center(int axis) {
  if (axis == 0) {
    return center_.x_;
  }
  if (axis == 1) {
    return center_.y_;
  }
  if (axis == 2) {
    return center_.z_;
  }
  return 0;
}
