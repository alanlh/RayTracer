#include "Drawable.h"

#include "../scene/Vector3.h"
#include "../image/HSLAPixel.h"

Drawable::Drawable() {
  surface_color_ = HSLAPixel(180, 0.5, 0.5, 1);
  emission_color_ = HSLAPixel(180, 0.5, 0.5, 1);
}

Drawable::~Drawable() {
}
