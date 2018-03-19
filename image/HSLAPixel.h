#ifndef HSLAPIXEL_H_
#define HSLAPIXEL_H_

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
 private:
  
};

#endif
