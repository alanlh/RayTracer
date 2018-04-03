#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>

#include "Vector3.h"
#include "../image/HSLAPixel.h"
#include "../objects/Drawable.h"
#include "../image/PNG.h"
#include "Ray.h"
#include "objects/ObjectTree.h"

class Scene {
 public:
  struct Camera {
    Vector3 point;
    double tiltAngle; // If 0, z-plane is down. If 90, z-plane is left
  };

  struct Canvas {
    // Defines the number of pixels in distance 1 away from the Canvas
    // TODO: Make this a function of the distance between Canvas/Camera
    // TODO: Should be able to modify this
    double zoomRatio;
    Vector3 center;
  };

  struct AmbientLight {
    HSLAPixel color;
    double intensity;
  };

  struct LightSource {
    Vector3 source;
    Vector3 direction;
    HSLAPixel color;
    double intensity;
  };
  
  /**
   * Creates an empty scene without a camera. 
   */
  Scene();
  
  /**
   * Destructor for a scene. Also destructs all objects within. 
   * Same as if removeAll is true.
   */
  ~Scene();
  /**
   * Deep copies another scene. 
   */
  Scene(Scene &other);
  
  Scene& operator=(Scene &other);

  void deleteObjects();

  /**
   * Adds a Drawable object to the scene.
   * The coordinates, etc., should be contained within the object. 
   */
  void AddDrawable(Drawable *object);
  
  /**
   * Goes through the list of objects and determines it is within it.
   * Removes if so. 
   */
  void RemoveObject(Drawable *object);

  void AddLightSource(LightSource *light);
  void RemoveLightSource(LightSource *light);

  void AddAmbientLight(AmbientLight *light);
  void RemoveAmbientLight(AmbientLight *light);

  void SetCamera(Camera cam, Canvas canvas);
  void SetCamera(Vector3 source, double tilt, Vector3 canvas, double zoom);
  
  /**
   * Renders the picture, and returns a collection of pixels
   * TODO: Figure out the return format
   */
  PNG * Render(unsigned width, unsigned height);

  PNG * RenderOrthographic(unsigned width, unsigned height);

 private:
  HSLAPixel GetPixColor(Ray ray, unsigned iteration);
  
  std::vector<Drawable *> *objects_;
  std::vector<LightSource *> *lights_;
  std::vector<AmbientLight *> *ambients_;
  struct Camera camera_;
  struct Canvas canvas_;

  //const unsigned MAX_ITERATION_COUNT = 1;
};

#endif
