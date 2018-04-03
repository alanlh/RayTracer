#include "HSLAPixel.h"

#include <cmath>
#include <iostream>

HSLAPixel::HSLAPixel() {
  l = 0.0;
  a = 1.0;
}

HSLAPixel::HSLAPixel(double hue,
		     double saturation,
		     double luminance) {
  h = hue;
  s = saturation;
  l = luminance;
  a = 1.0;
}

HSLAPixel::HSLAPixel(double hue,
		     double saturation,
		     double luminance,
		     double alpha) {
  h = hue;
  s = saturation;
  l = luminance;
  a = alpha; 
}

HSLAPixel HSLAPixel::operator*(HSLAPixel other) {
  return ToHSL(getRed() * other.getRed(),
	       getGreen() * other.getGreen(),
	       getBlue() * other.getBlue());
}

HSLAPixel HSLAPixel::operator*(double val) {
  double hue = h;
  double sat = s;
  double lum = l;

  lum *= val;
  if (lum > 1) {
    lum = 1;
  }
  if (1 - fabs(2 * lum - 1) == 0) {
    sat = 0;
  } else {
    sat *= val * (1 - fabs( 2 * l - 1)) / (1 - fabs(2 * lum - 1));
  }
  if (sat > 1) {
    sat = 1;
  }
  return HSLAPixel(hue, sat, lum);
}

HSLAPixel HSLAPixel::operator+(HSLAPixel other) {
  return ToHSL(sigmoid(inverse(getRed()) + inverse(other.getRed())),
	       sigmoid(inverse(getGreen()) + inverse(other.getGreen())),
	       sigmoid(inverse(getBlue()) + inverse(other.getBlue())));
}

double HSLAPixel::getRed() {
  return ToRGB((h + 120) / 360);
}

double HSLAPixel::getGreen() {
  return ToRGB(h / 360);
}

double HSLAPixel::getBlue() {
  return ToRGB((h - 120) / 360);
}

double HSLAPixel::ToRGB(double hue) {
  if (s == 0) {
    return l;
  }
  if (hue > 1) {
    hue -= 1;
  }
  if (hue < 0) {
    hue += 1;
  }
  double temp1, temp2;
  if (l < 0.5) {
    temp2 = l * (1 + s);
  }
  else {
    temp2 = (l + s) - (s * l);
  }
  
  temp1 = 2 * l - temp2;
  
  if (hue < 0.16667) {
    return temp1 + (temp2 - temp1) * 6 * hue;
  }
  if (hue < 0.5) {
    return temp2;
  }
  if (hue < 0.666667) {
    return temp1 + (temp2 - temp1) * 6 * (0.66666667 - hue);
  }
  return temp1;
}

HSLAPixel HSLAPixel::ToHSL(double r, double g, double b) {
  double cmax = fmax(r, g);
  cmax = fmax(cmax, b);

  double cmin = fmin(r, g);
  cmin = fmin(cmin, b);
  
  double hue = 0;

  double delta = cmax - cmin;

  if (delta == 0) {
    hue = 0;
  } else if (cmax == r) {
    hue = (g - b < 0) ? 360 + (g - b) * 60 / delta : (g - b) * 60 / delta;
  } else if (cmax == g) {
    hue =  (b - r) * 60 / delta + 120;
  } else {
    hue = (r - g) * 60 / delta + 240;
  }

  double lum = (cmax + cmin) / 2;
  double sat = (lum == 0 || lum == 1) ? 0 : delta / (1 - fabs(2 * l - 1));
  return HSLAPixel(hue, sat, lum);
}

double HSLAPixel::sigmoid(double in) {
  double val = (2 / (1 + exp(-1 * in))) - 1;
  return val;
}

double HSLAPixel::inverse(double in) {
  double val = -1 * log((2 / (in + 1)) - 1);
  return val;
}

std::ostream& operator<<(std::ostream& os, HSLAPixel &pixel) {
  return os << "(" << pixel.h << ", " << pixel.s << ", " << pixel.l << ")";
}
