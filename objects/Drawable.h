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
  double ambient_;
  double diffusion_;
  double specular_;
  double shininess_; // Determines how shiny something is
  double reflection_; // Unused for now
  double transparency_; // Unused for now
  double refraction_constant_; // Unused for now
};

#endif
