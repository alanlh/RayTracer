#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "../scene/Vector3.h"
#include "../image/HSLAPixel.h"
#include "../scene/Ray.h"

class Drawable {
 public:
  Drawable();

  virtual ~Drawable();

  /**
   * Determines whether the ray intersects the object
   */
  virtual double Intersects(Ray ray) = 0;
  /**
   * Returns a perpendicular vector to the surface of the object
   * At the location determined by the ray and distance
   */
  virtual Vector3 GetPerpendicular(Ray ray, double d) = 0;

  /**
   * Returns a Vector3 with the minimum possible coordinates in all 3 dimens
   * Used to create bounding volumes
   */
  virtual Vector3 min() = 0;
  
  /**
   * Returns a Vector3 with the maximum possible coordinates in all 3 dimens
   * Used to create bounding volumes
   */
  virtual Vector3 max() = 0;

  /**
   * Returns the "center" of the object. 
   * Used for dividing bounding volumes. 
   * Axis is from 0 to 2. x_ = 0, y_ = 1, z_ = 2
   */
  virtual double center(int axis) = 0;
  
  HSLAPixel surface_color_;
  double ambient_; // Used to determine how much ambient light is reflected
  double diffusion_; // Used to determine how much diffuse light is reflected

  // Everything below is currently not implemented //
  double specular_;  // Used to determine specular reflection
  double shininess_; // Determines how shiny something is
  double reflection_; // Unused for now
  double transparency_; // Unused for now
  double refraction_constant_; // Unused for now
};

#endif
