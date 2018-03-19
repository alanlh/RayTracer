#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

class Ray {
 public:
  Ray(Vector3 s, Vector3 d) {
    source = s;
    direction = d;
  }

  Vector3 point(double d) {
    return source + direction * d;
  }
  
  Vector3 source;
  Vector3 direction;
};

#endif
