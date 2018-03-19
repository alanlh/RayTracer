#include "ColorMixer.h"

#include <iostream>
#include "math.h"

#include "HSLAPixel.h"

ColorMixer::ColorMixer() {
  surface_weight_ = default_surface_weight_;
  light_weight_ = default_light_weight_;
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
  surface_weight_ = other.surface_weight_;
  light_weight_ = other.light_weight_;
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
  surface_weight_ = other.surface_weight_;
  light_weight_ = other.light_weight_;
  return *this;
}

void ColorMixer::AddColor(HSLAPixel pix, unsigned type,
			  double intensity) {
  colors.push_back(pix);
  types.push_back(type);
  intensities.push_back(intensity);
}

HSLAPixel ColorMixer::RenderObjectColor() {
  double h = 0;
  double h_weight = 0;
  double s = 0;
  double s_weight = 0;
  double l = 0;

  double h_object = 0;
  double s_object = 0;
  double l_object = 0;

  double ambient_l = 0;

  bool is_object = false;
  unsigned light_count = 0;

  //double intensity = 0;
  //double intensity_weight = 0;

  for (unsigned i = 0; i < colors.size(); i ++) {
    // A variation on the Phong reflection model
    // Illumination is similar, but I'm using my own color/sat mixing system
    // TODO: AFTER ADDING INTENSITY, MOVE LUMINOSITY BACK.
    if (types.at(i) == surface_color_) {
      // Should only occur once
      // TODO: Create constants. For now, all weights for surface color is 1
      h_object = colors.at(i).h;
      s_object = colors.at(i).s;
      l_object = colors.at(i).l;
      is_object = true;
    } else if (types.at(i) == directional_light_) {
      // H dependent on S and L, S dependent on L. L is a lot more weighted
      // h += 0.2 * intensities.at(i) * colors.at(i).h;
      // h_weight += 0.2 * intensities.at(i);
      // s += 0.2 * intensities.at(i) * colors.at(i).s;
      // s_weight += 0.2 * intensities.at(i);
      //h += 2 * intensities.at(i) * colors.at(i).h;
      //h_weight += 2 * intensities.at(i);
      //s += 2 * intensities.at(i) * colors.at(i).s;
      //s_weight += 2 * intensities.at(i);
      l += intensities.at(i) * colors.at(i).l;
      light_count ++;
      //intensity += 5 * intensities.at(i);
      //intensity_weight += 5;

    } else if (types.at(i) == ambient_light_) {
      // Similar to directional light, but less weighted. 
      //h += 0.05 * intensities.at(i) * colors.at(i).h;
      //h_weight += 0.05 * intensities.at(i);
      //s += 0.05 * intensities.at(i) * colors.at(i).s;
      //s_weight += 0.05 * intensities.at(i);
      h += 2 * intensities.at(i) * colors.at(i).h;
      h_weight += 2 * intensities.at(i);
      s += 2 * intensities.at(i) * colors.at(i).s;
      s_weight += 2 * intensities.at(i);
      l += intensities.at(i) * colors.at(i).l;

      ambient_l += intensities.at(i) * colors.at(i).l;
      light_count ++;
      //intensity += 2 * intensities.at(i);
      //intensity_weight += 2;
    }    
  }

  if (h_weight > 0) {
    h /= h_weight;
  }
  if (s_weight > 0) {
    s /= s_weight;
  }
  if (light_count > 0 && !is_object) {
    l /= light_count;
  }
  if (is_object && l > 1) {
    l = 1;
  }
  //if (intensity_weight > 0) {
  // intensity /= intensity_weight;
  //}
  //l = sigmoid(0.5 * inverse(l) + 1.5 * intensity);
  //l = sigmoid(2 * intensity);
  // std::cout << l << " " << l_object std::endl;
  
  // if (!is_object) {
  //   return HSLAPixel(h, s, l);
  // }

  l = sigmoid(inverse(ambient_l * l_object) + inverse(l));
  
  return HSLAPixel(h_object, s_object, l);
}

HSLAPixel ColorMixer::RenderAntiAlias() {
  double prominent_hue = 0;
  double max_l = 0;
  for (HSLAPixel color : colors) {
    if (color.l > max_l) {
      max_l = color.l;
      prominent_hue = color.h;
    }
  }

  double s = 0;
  double l = 0;
  unsigned count = 0;
  for (HSLAPixel color : colors) {
    s += color.s;
    l += color.l;
    count ++;
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
