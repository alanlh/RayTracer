#include "HSLAPixel.h"

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
