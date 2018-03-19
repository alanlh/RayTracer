#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "../scene/Vector3.h"
#include "../image/HSLAPixel.h"
#include "../scene/Ray.h"

class Drawable {
 public:
  Drawable();

  virtual ~Drawable();
  virtual double Intersects(Ray ray) = 0;
  virtual Vector3 GetPerpendicular(Ray ray, double d) = 0;

  HSLAPixel surface_color_;
  HSLAPixel emission_color_;
  double reflection_;
  double transparency_;
};

#endif
