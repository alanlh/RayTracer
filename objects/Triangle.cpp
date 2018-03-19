#include "Triangle.h"
#include "Drawable.h"

#include <math.h>
#include <iostream>

#include "../scene/Vector3.h"
#include "../scene/Ray.h"

Triangle::Triangle() {
  a_ = Vector3(1, 0, 0);
  b_ = Vector3(0, 1, 0);
  c_ = Vector3(0, 0, 1);
}

Triangle::~Triangle() {

}

double Triangle::Intersects(Ray ray) {
  if (dotProduct(ray.direction, crossProduct(b_ - a_, c_ - a_)) < 0.001) {
    return -1;
    // If ray is parallel to plane, then will never intersect
  }
  Vector3 n = GetPerpendicular(ray, 0);
  double distance = (dotProduct(n, ray.source + a_))
    / (dotProduct(n, ray.direction));
  if (distance < 0) {
    return -1;
  }

  Vector3 intersectPoint = ray.point(distance);

  Vector3 edge1 = b_ - a_;
  Vector3 edge2 = c_ - b_;
  Vector3 edge3 = a_ - c_;
  Vector3 p1 = intersectPoint - a_;
  Vector3 p2 = intersectPoint - b_;
  Vector3 p3 = intersectPoint - c_;
  //std::cout << dotProduct(n, crossProduct(edge1, p1)) << std::endl;
  // std::cout << dotProduct(n, crossProduct(edge2, p2)) << std::endl;
  // std::cout << dotProduct(n, crossProduct(edge3, p3)) << std::endl;
  // std::cout << std::endl;
  
  // Used formula from
  // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
  if (dotProduct(n, crossProduct(edge1, p1)) < 0 && 
      dotProduct(n, crossProduct(edge2, p2)) < 0 && 
      dotProduct(n, crossProduct(edge3, p3)) < 0) {
    return distance;
  } else {
    return -1;
  }
}

Vector3 Triangle::GetPerpendicular(Ray ray, double d) {
  d = 0;
  Vector3 normal = crossProduct(b_ - a_, c_ - a_);
  if (dotProduct(ray.direction, normal) > 0) {
    normal = normal * -1;
  }
  return normal;
}