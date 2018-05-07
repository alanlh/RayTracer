#include "Drawable.h"

#include "../scene/Vector3.h"
#include "../image/HSLAPixel.h"

Drawable::Drawable() {
  surface_color_ = HSLAPixel(180, 0.5, 0.5, 1);
  ambient_ = 0.1;
  diffusion_ = 0.18;
  specular_ = 0.1;
  shininess_ = 10;
  reflection_ = 1;
}

Drawable::~Drawable() {
}
