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
#include "../objects/ObjectTree.h"
#include "Parser.h"

Scene::Scene() {
  objects_ = new vector<Drawable *>;
  lights_ = new vector<LightSource *>;
  ambients_ = new vector<AmbientLight *>;

  tree_ = NULL;
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
    if (*it != NULL) {
      delete (*it);
      (*it) = NULL;
    }
  }
  
  for (std::vector<LightSource *>::iterator it = lights_->begin();
       it != lights_->end(); ++ it) {
    if (*it != NULL) {
      delete (*it);
      (*it) = NULL;
    }
  }
  
  for (std::vector<AmbientLight *>::iterator it = ambients_->begin();
       it != ambients_->end(); ++ it) {
    if (*it != NULL) {
      delete (*it);
      (*it) = NULL;
    }
  }
  
  delete objects_;
  delete lights_;
  delete ambients_;
}

void Scene::AddDrawable(Drawable *object) {
  objects_->push_back(object);
}

void Scene::ParseObj(std::string path) {
  vector<Drawable *> *objects = Parser::ParseObjects(path);
  for (Drawable * drawable : *objects) {
    objects_->push_back(drawable);
  }

  // Should not delete the objects
  delete objects;
}

/**
 * DO NOT CALL. NOT BEING UPDATED FOR BVHs
 */
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

  tree_ = new ObjectTree(*objects_);

  Vector3 point_direction = canvas_.center;
  point_direction = point_direction - camera_.point;

  //double canvas_distance = point_direction.magnitude();
  
  Vector3 z_plane = point_direction;
  z_plane.pointTowardsYPos();
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
  delete tree_;
  tree_ = NULL;
  
  return image;
}

PNG * Scene::RenderOrthographic(unsigned width, unsigned height) {
  PNG *image = new PNG(width, height);

  tree_ = new ObjectTree(*objects_);

  Vector3 point_direction = canvas_.center - camera_.point;

  //double canvas_distance = point_direction.magnitude();
  
  Vector3 z_plane = point_direction;
  z_plane.pointTowardsYPos();
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
	Vector3 pixel_x_offset = rightXDirection * (i + x_rand - w/2 + 1/2)
	  * (1 / canvas_.zoomRatio);
	Vector3 pixel_y_offset = topYDirection * (j + y_rand- h/2 + 1/2)
	  * (-1 / canvas_.zoomRatio);
	
	Vector3 pixel_coordinate = camera_.point
	  + pixel_x_offset + pixel_y_offset;
	Ray ray(pixel_coordinate, canvas_.center - camera_.point);
	HSLAPixel color = GetPixColor(ray, 1);
	final_pix.AddColor(color, 0, 1);
      }
      HSLAPixel& image_pix = *(image->getPixel(i, j));
      image_pix = final_pix.RenderAntiAlias();
    }
  }
  delete tree_;
  tree_ = NULL;
  
  return image;
}

HSLAPixel Scene::GetPixColor(Ray ray, unsigned iteration) {
  Drawable *closestObject = tree_->intersect(ray);
  ColorMixer pix;

  bool reflectionExists = false;
  HSLAPixel reflectionPixel;

  for (AmbientLight *ambient : *ambients_) {
    double intensity = ambient->intensity;
    if (closestObject != NULL) {
      intensity *= closestObject->ambient_;
    }
    pix.AddColor(ambient->color, ColorMixer::light_, intensity);
  }
  
  if (closestObject != NULL) {
    double minDistance = closestObject->Intersects(ray);
    pix.AddObject(closestObject);

    Vector3 perp = closestObject->GetPerpendicular(ray, minDistance);
    Vector3 reflection = perp * (dotProduct(perp, ray.direction) * -2 /
				 (perp.magnitude() * ray.direction.magnitude()))
      + ray.direction;
    
    // In direction away from surface
    for (LightSource *light : *lights_) {
      // If light->direction = 0, then create vector by comparing the start point.

      Ray lightReached(ray.point(minDistance) - light->direction * 0.001, light->direction * -1);

        if (tree_->intersect(lightReached) == NULL) {
      
	double diffuse_weight = dotProduct(perp, light->direction) /
	  (perp.magnitude() * light->direction.magnitude());
	if (diffuse_weight < 0) {
	// TODO: Implement operator* on HSLAPixel
	  pix.AddColor(light->color, ColorMixer::light_,
		       light->intensity * closestObject->diffusion_ * diffuse_weight * -1);
	} else {
	  pix.AddColor(light->color, ColorMixer::light_, 0);
	}
	double specular_weight = -1 * dotProduct(ray.direction, reflection) /
	  (ray.direction.magnitude() * reflection.magnitude());
	if (specular_weight > 0) {
	  pix.AddColor(light->color, ColorMixer::light_,
		       closestObject->specular_ * light->intensity *
		       pow(specular_weight, closestObject->shininess_));
	}
	
	}
    }    
    Ray reflection_ray(ray.point(minDistance) + reflection * 0.01, reflection);

    if (iteration < 2 && tree_->intersect(reflection_ray) != NULL) {
      reflectionExists = true;
      reflectionPixel = GetPixColor(reflection_ray, iteration + 1);
      /*
      pix.AddColor(reflectionPixel, ColorMixer::reflection_,
      closestObject->reflection_);*/
    } 
  }
  
  //return pix.RenderObjectColor();
  
  HSLAPixel naturalColor = pix.RenderObjectColor();
  ColorMixer final;
  if (closestObject != NULL && reflectionExists) {
      final.AddColor(naturalColor, ColorMixer::light_, 1 - closestObject->reflection_);
      final.AddColor(reflectionPixel, ColorMixer::light_, closestObject->reflection_);
  } else {
    return naturalColor;
  }
  // TODO: Change name of function or adapt it. 
  return final.RenderAntiAlias();
  
}
