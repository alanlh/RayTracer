#include "Scene.h"

#include <vector>
#include <iostream>
#include "math.h"
#include <cstdlib>

#include "../image/PNG.h"
#include "../image/HSLAPixel.h"
#include "../objects/Drawable.h"
#include "Ray.h"
#include "Vector3.h"
#include "../image/ColorMixer.h"

Scene::Scene() {
  objects_ = new vector<Drawable *>;
  lights_ = new vector<LightSource *>;
  ambients_ = new vector<AmbientLight *>;
}

Scene::Scene(Scene &other) {
  objects_ = new vector<Drawable *>;
  lights_ = new vector<LightSource *>;
  ambients_ = new vector<AmbientLight *>;

  for (unsigned i = 0; i < other.objects_->size(); i ++) {
    objects_->push_back(other.objects_->at(i));
  }
  for (unsigned i = 0; i < other.lights_->size(); i ++) {
    lights_->push_back(other.lights_->at(i));
  }
  for (unsigned i = 0; i < other.ambients_->size(); i ++) {
    ambients_->push_back(other.ambients_->at(i));
  }
  camera_.point = other.camera_.point;
  camera_.tiltAngle = other.camera_.tiltAngle;
  canvas_.center = other.canvas_.center;
  canvas_.zoomRatio = other.canvas_.zoomRatio;
}

Scene& Scene::operator=(Scene &other) {
  if (this == &other) {
    return *this;
  }

  deleteObjects();
  
  objects_ = new vector<Drawable *>;
  lights_ = new vector<LightSource *>;
  ambients_ = new vector<AmbientLight *>;

  for (unsigned i = 0; i < other.objects_->size(); i ++) {
    objects_->push_back(other.objects_->at(i));
  }
  for (unsigned i = 0; i < other.lights_->size(); i ++) {
    lights_->push_back(other.lights_->at(i));
  }
  for (unsigned i = 0; i < other.ambients_->size(); i ++) {
    ambients_->push_back(other.ambients_->at(i));
  }
  camera_.point = other.camera_.point;
  camera_.tiltAngle = other.camera_.tiltAngle;
  canvas_.center = other.canvas_.center;
  canvas_.zoomRatio = other.canvas_.zoomRatio;

  return *this;
}

Scene::~Scene() {
  deleteObjects();
}

void Scene::deleteObjects() {
  for (std::vector<Drawable *>::iterator it = objects_->begin();
       it != objects_->end(); ++ it) {
    delete (*it);
  }
  
  for (std::vector<LightSource *>::iterator it = lights_->begin();
       it != lights_->end(); ++ it) {
    delete (*it);
  }
  
  for (std::vector<AmbientLight *>::iterator it = ambients_->begin();
       it != ambients_->end(); ++ it) {
    delete (*it);
  }
  
  delete objects_;
  delete lights_;
  delete ambients_;
}

void Scene::AddDrawable(Drawable *object) {
  objects_->push_back(object);
}

void Scene::RemoveDrawable(Drawable *object) {
  for (unsigned i = 0; i < objects_->size(); i ++) {
    if (objects_->at(i) == object) {
      objects_->erase(objects_->begin() + i);
      break;
    }
  }
}

void Scene::AddLightSource(LightSource *light) {
  lights_->push_back(light);
}

void Scene::RemoveLightSource(LightSource *light) {
  for (unsigned i = 0; i < lights_->size(); i ++) {
    if (lights_->at(i) == light) {
      lights_->erase(lights_->begin() + i);
      break;
    }
  }
}

void Scene::AddAmbientLight(AmbientLight *light) {
  ambients_->push_back(light);
}

void Scene::RemoveAmbientLight(AmbientLight *light) {
  for (unsigned i = 0; i < ambients_->size(); i ++) {
    if (ambients_->at(i) == light) {
      ambients_->erase(ambients_->begin() + i);
      break;
    }
  }
}

void Scene::SetCamera(Camera cam, Canvas canvas) {
  camera_ = cam;
  canvas_ = canvas;
}

void Scene::SetCamera(Vector3 source, double tilt, Vector3 canvas, double zoom) {
  camera_.point = source;
  camera_.tiltAngle = tilt;
  canvas_.center = canvas;
  canvas_.zoomRatio = zoom;
}

PNG * Scene::Render(unsigned width, unsigned height) {
  PNG *image = new PNG(width, height);

  Vector3 point_direction = canvas_.center;
  point_direction = point_direction - camera_.point;

  //double canvas_distance = point_direction.magnitude();
  
  Vector3 z_plane = point_direction;
  z_plane.pointTowardsZPos();
  Vector3 topYDirection = z_plane;
  topYDirection.rotate(camera_.tiltAngle, point_direction);
  Vector3 rightXDirection = topYDirection;
  rightXDirection.rotate(270, point_direction);
  
  // TODO: Check if pointDirection is vertical. 
  int w = (int) width;
  int h = (int) height;
  
  for (int i = 0; i < w; i ++) {
    for (int j = 0; j < h; j ++) {
      // TODO: Create separate methods to do multiple times and with jitter
      int rpp = 4; // Rays Per Pixel
      ColorMixer final_pix;
      for (int k = 0; k < rpp; k ++) {
	double x_rand = ((double) rand() / RAND_MAX) - 0.5;
	double y_rand = ((double) rand() / RAND_MAX) - 0.5;
	Vector3 pixel_x_offset = rightXDirection * (i + x_rand - w/2 + 1/2) * (1 / canvas_.zoomRatio);
	Vector3 pixel_y_offset = topYDirection * (j + y_rand- h/2 + 1/2) * (-1 / canvas_.zoomRatio);
	
	Vector3 pixel_coordinate = canvas_.center + pixel_x_offset + pixel_y_offset;
	Ray ray(camera_.point, pixel_coordinate - camera_.point);
	HSLAPixel color = GetPixColor(ray, 1);

	final_pix.AddColor(color, 0, color.l);

      }
      HSLAPixel& image_pix = *(image->getPixel(i, j));
      image_pix = final_pix.RenderAntiAlias();
    }
  }
  return image;
}

PNG * Scene::RenderOrthographic(unsigned width, unsigned height) {
  PNG *image = new PNG(width, height);

  Vector3 point_direction = canvas_.center - camera_.point;

  //double canvas_distance = point_direction.magnitude();
  
  Vector3 z_plane = point_direction;
  z_plane.pointTowardsZPos();
  Vector3 topYDirection = z_plane;
  topYDirection.rotate(camera_.tiltAngle, point_direction);
  Vector3 rightXDirection = topYDirection;
  rightXDirection.rotate(270, point_direction);
  
  // TODO: Check if pointDirection is vertical. 
  int w = (int) width;
  int h = (int) height;
  
  for (int i = 0; i < w; i ++) {
    for (int j = 0; j < h; j ++) {
      // TODO: Create separate methods to do multiple times and with jitter
      int rpp = 4; // Rays Per Pixel
      ColorMixer final_pix;
      for (int k = 0; k < rpp; k ++) {
	double x_rand = ((double) rand() / RAND_MAX) - 0.5;
	double y_rand = ((double) rand() / RAND_MAX) - 0.5;
	Vector3 pixel_x_offset = rightXDirection * (i + x_rand - w/2 + 1/2) * (1 / canvas_.zoomRatio);
	Vector3 pixel_y_offset = topYDirection * (j + y_rand- h/2 + 1/2) * (-1 / canvas_.zoomRatio);
	
	Vector3 pixel_coordinate = camera_.point + pixel_x_offset + pixel_y_offset;
	Ray ray(pixel_coordinate, canvas_.center - camera_.point);
	HSLAPixel color = GetPixColor(ray, 1);
	final_pix.AddColor(color, 0, color.l);
      }
      HSLAPixel& image_pix = *(image->getPixel(i, j));
      image_pix = final_pix.RenderAntiAlias();
    }
  }
  return image;
}

HSLAPixel Scene::GetPixColor(Ray ray, unsigned iteration) {
  if (iteration > 3) { // REPLACE WITH MAX_ITERATION_COUNT
    // For shadows, reflection, etc.
    return HSLAPixel();
  }
  double minDistance = 0;
  Drawable *closestObject = NULL;
  for (Drawable *object : *objects_) {
    double distance = object->Intersects(ray);
    if (minDistance <= 0 && distance >= 0) {
      minDistance = distance;
      closestObject = object;
    } else if (distance > 0) {
      if (distance < minDistance) {
	minDistance = distance;
	closestObject = object;
      }
    }
  }
  ColorMixer pix;
  if (closestObject != NULL) {
    pix.AddColor(closestObject->surface_color_, ColorMixer::surface_color_, 0);
    Vector3 perp = closestObject->GetPerpendicular(ray, minDistance);
    for (LightSource *light : *lights_) {
      double weight = dotProduct(perp, light->direction) /
	(perp.magnitude() * light->direction.magnitude());
      if (weight < 0) {
	// TODO: Implement operator* on HSLAPixel
	double intensity = light->intensity * weight * -1;
	pix.AddColor(light->color, ColorMixer::directional_light_, intensity);
      } else {
	//cs225::HSLAPixel unseen_light = cs225::HSLAPixel(light->color.h, light->color.s, 0);
	//pix.AddColor(unseen_light, ColorMixer::directional_light_);
      }
    }
  }
  for (AmbientLight *ambient : *ambients_) {
    pix.AddColor(ambient->color, ColorMixer::ambient_light_, ambient->intensity);
  }
  return pix.RenderObjectColor();
}
