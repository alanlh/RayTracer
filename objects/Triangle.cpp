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
  double plane_angle = dotProduct(ray.direction,
				  crossProduct(b_ - a_, c_ - a_)) < 0.001;

  if (plane_angle < 0.001 && plane_angle > 0.001) {
    return -1;
    // If ray is parallel to plane, then will never intersect
  }

  // Check where object intersects plane
  Vector3 n = GetPerpendicular(ray, 0);
  double distance = (dotProduct(n, ray.source + a_))
    / (dotProduct(n, ray.direction));
  if (distance < 0) {
    return -1;
  }

  // CHeck intersect object
  Vector3 intersectPoint = ray.point(distance);
  Vector3 edge1 = b_ - a_;
  Vector3 edge2 = c_ - b_;
  Vector3 edge3 = a_ - c_;
  Vector3 p1 = intersectPoint - a_;
  Vector3 p2 = intersectPoint - b_;
  Vector3 p3 = intersectPoint - c_;
  
  // Used formula from
  // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
  if ((dotProduct(n, crossProduct(edge1, p1)) < 0.001 && 
      dotProduct(n, crossProduct(edge2, p2)) < 0.001 && 
       dotProduct(n, crossProduct(edge3, p3)) < 0.001) ||
      (dotProduct(n, crossProduct(edge1, p1)) > -0.001 && 
       dotProduct(n, crossProduct(edge2, p2)) > -0.001 && 
       dotProduct(n, crossProduct(edge3, p3)) > -0.001)) {
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

Vector3 Triangle::min() {
  return Vector3(std::min(a_.x_, std::min(b_.x_, c_.x_)),
		 std::min(a_.y_, std::min(b_.y_, c_.y_)),
		 std::min(a_.z_, std::min(b_.z_, c_.z_)));
}

Vector3 Triangle::max() {
  return Vector3(std::max(a_.x_, std::max(b_.x_, c_.x_)),
		 std::max(a_.y_, std::max(b_.y_, c_.y_)),
		 std::max(a_.z_, std::max(b_.z_, c_.z_)));
}

double Triangle::center(int axis) {
  if (axis == 0) {
    return (a_.x_ + b_.x_ + c_.x_) / 3;
  }
  if (axis == 1) {
    return (a_.y_ + b_.y_ + c_.y_) / 3;
  }
  if (axis == 2) {
    return (a_.z_ + b_.z_ + c_.z_) / 3;
  }
  return 0;
}
