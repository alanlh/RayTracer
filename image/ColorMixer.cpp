#include "ColorMixer.h"

#include <iostream>
#include "math.h"

#include "HSLAPixel.h"
#include "../objects/Drawable.h"

ColorMixer::ColorMixer() {
  object_ = NULL;
}

ColorMixer::ColorMixer(ColorMixer &other) {
  for (HSLAPixel color : other.colors) {
    colors.push_back(color);
  }
  for (unsigned type : other.types) {
    types.push_back(type);
  }
  for (double intensity : other.intensities) {
    intensities.push_back(intensity);
  }
  object_ = other.object_;
}

ColorMixer & ColorMixer::operator=(ColorMixer &other) {
  colors.clear();
  types.clear();
  intensities.clear();
  for (HSLAPixel color : other.colors) {
    colors.push_back(color);
  }
  for (unsigned type : other.types) {
    types.push_back(type);
  }
  for (double intensity : other.intensities) {
    intensities.push_back(intensity);
  }
  object_ = other.object_;
  return *this;
}

void ColorMixer::AddObject(Drawable *object) {
  object_ = object;
}

void ColorMixer::AddColor(HSLAPixel pix, unsigned type,
			  double intensity) {
  colors.push_back(pix);
  types.push_back(type);
  intensities.push_back(intensity);
}

HSLAPixel ColorMixer::RenderObjectColor() {
  bool is_object = (object_ != NULL);

  HSLAPixel object_color;
  if (is_object) {
    object_color = object_->surface_color_;
  }

  // bool small_change = false;

  if (colors.size() == 0) {
    return HSLAPixel();
  }

  HSLAPixel light = colors[0] * intensities[0];
  
  // if (intensities[0] < 0.5) {
  //   std::cout << intensities[0] << " " << colors[0] << std::endl;
  //   small_change = true;
  // }
  for (unsigned i = 1; i < colors.size(); i ++) {
    //if (types[i] == ColorMixer::light_) {
    if (intensities[i] > 0) {
      light = light + colors[i] * intensities[i];
    }
      //}
    // if (intensities[i] < 0.5) {
    //   std::cout << intensities[i] << " " << colors[i] << std::endl;
    //   small_change = true;
    // }
  }
  HSLAPixel result = object_color * light;

  // if (small_change) {
  //   std::cout << object_color << " " << light << " " << result << std::endl;
  // }
  
  return result;
}

HSLAPixel ColorMixer::RenderAntiAlias() {
  double prominent_hue = 0;
  double max_l = 0;

  for (unsigned i = 0; i < colors.size(); i ++) {
    if (colors[i].l * intensities[i] > max_l) {
      max_l = colors[i].l * intensities[i];
      prominent_hue = colors[i].h;
    }
  }

  double s = 0;
  double l = 0;
  double count = 0;
  for (unsigned i = 0; i < colors.size(); i ++) {
    // if (fabs(color.h - prominent_hue) < 60) {    
      s += colors[i].s * intensities[i];
      l += colors[i].l * intensities[i];
      count += intensities[i];
      //}
  }
  if (count > 0) {
    s /= count;
    l /= count;
  }  
  // Blend luminosities and saturation, but keep hue. 
  return HSLAPixel(prominent_hue, s, l);
}

double ColorMixer::sigmoid(double in) {
  return (2 / (1 + exp(-1 * in))) - 1;
}

double ColorMixer::inverse(double in) {
  return -1 * log((2 / (in + 1)) - 1);
}
