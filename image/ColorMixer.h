#ifndef COLORMIXER_H
#define COLORMIXER_H

#include "HSLAPixel.h"

#include <vector>

#include "../objects/Drawable.h"

class ColorMixer {
 public:
  ColorMixer();
  ColorMixer(ColorMixer &other);

  ColorMixer& operator=(ColorMixer &other);

  void AddObject(Drawable *object);
  void AddColor(HSLAPixel pix, unsigned type, double intensity);
  void SetObjectInfo(Drawable *object);

  /** Mixes a set of colors by weights determined by type **/
  HSLAPixel RenderObjectColor();
  /** For anti-aliasing, when there are two objects mixed **/
  /** Picks whichever light has the highest luminosity **/
  HSLAPixel RenderAntiAlias();
  
  const static unsigned default_color_ = 0;
  const static unsigned light_ = 0; // Ambient and other lights
  const static unsigned reflection_ = 1;
  // Other sources: transparency? Or 
  
 private:
  std::vector<HSLAPixel> colors;
  std::vector<unsigned> types;
  std::vector<double> intensities;

  Drawable *object_;

  double sigmoid(double in);
  double inverse(double in);
};

#endif
