#ifndef HSLAPIXEL_H_
#define HSLAPIXEL_H_

#include <iostream>

class HSLAPixel {
 public:
  double h;
  double s;
  double l;
  double a;
  
  HSLAPixel();
  HSLAPixel(double hue,
	    double saturation,
	    double luminance);
  HSLAPixel(double hue,
	    double saturation,
	    double luminance,
	    double alpha);

  HSLAPixel operator*(HSLAPixel other);
  HSLAPixel operator*(double val);
  HSLAPixel operator+(HSLAPixel other);

  
 private:
  double getRed();
  double getGreen();
  double getBlue();

  HSLAPixel ToHSL(double r, double g, double b);
  double ToRGB(double hue);

  double sigmoid(double in);
  double inverse(double in);
};

std::ostream& operator<<(std::ostream& os, HSLAPixel &pix);

#endif
