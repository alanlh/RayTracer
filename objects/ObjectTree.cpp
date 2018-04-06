#include "ObjectTree.h"

#include <vector>
#include <iostream>

#include "../scene/Ray.h"
#include "Drawable.h"
#include "../scene/Vector3.h"

ObjectTree::ObjectTree(std::vector<Drawable *> & objects) {
  root_ = NULL;
  if (objects.size() == 0) {
    return;
  }
  if (objects.size() == 1) {
    root_ = new ObjectTreeNode(objects[0]);
    return;
  }

  root_ = construct(objects, 0, objects.size() - 1, 0);
}

ObjectTree::ObjectTreeNode *ObjectTree::construct(std::vector<Drawable *> &objects,
				      int lo, int hi, int axis) {
  if (lo == hi) {
    return new ObjectTreeNode(objects[lo]);
  }
  partition(objects, lo, hi, axis);
  ObjectTreeNode *left = construct(objects, lo, (lo + hi) / 2, (axis + 1) % 3);
  ObjectTreeNode *right = construct(objects, (lo + hi) / 2 + 1, hi,
				    (axis + 1) % 3);
  
  return new ObjectTreeNode(left, right);
}

/**
 * Partitions objects
 * All objects smaller than split will be on one side
 * All objects larger than line will be on other side
 */
void ObjectTree::partition(std::vector<Drawable *> &objects,
			   int lo, int hi, int axis) {
  if (lo >= hi) {
    return;
  }
  int middle = (lo + hi) / 2;
  int new_index = 0;
  while (new_index != middle) {
    swap(objects, middle, hi);
    int lower = lo;
    int higher = hi - 1;
    while (lower < higher) {
      if (objects[lower]->center(axis) > objects[higher]->center(axis)) {
	while (objects[higher]->center(axis) > objects[hi]->center(axis)) {
	  higher --;
	  if (lower == higher) {
	    swap(objects, higher, hi);
	    return;
	  }
	}
	swap(objects, lower, higher);
      }
      lower ++;
    }
    swap(objects, higher, hi);

    new_index = higher;
    lo = (new_index < middle) ? new_index : lo;
    hi = (new_index < middle) ? hi : new_index;
  }
}

void ObjectTree::swap(std::vector<Drawable *> &objects, int lo, int hi) {
  Drawable *temp = objects[lo];
  objects[lo] = objects[hi];
  objects[hi] = temp;
}

ObjectTree::~ObjectTree() {
  destroy();
}

/** DO NOT USE **/
ObjectTree::ObjectTree(ObjectTree &other) {
  copy(other);
}

/** DO NOT USE **/
ObjectTree & ObjectTree::operator=(ObjectTree &other) {
  if (&other != this) {
    destroy();
    copy(other);
  }
  return *this;
}

void ObjectTree::copy(ObjectTree &other) {
  *root_ = *other.root_;
}

void ObjectTree::destroy() {
  if (root_ != NULL) {
    delete root_;
  }
}

Drawable *ObjectTree::intersect(Ray ray) {
  return root_->findDrawable(ray);
}

void ObjectTree::print() {
  if (root_ == NULL) {
    std::cout << "Root NULL" << std::endl;
    return;
  }
  root_->print(0);
}

ObjectTree::ObjectTreeNode::ObjectTreeNode(Drawable *object) {
  object_ = object;
  left_ = NULL;
  right_ = NULL;
  min_ = object->min();
  max_ = object->max();
}

ObjectTree::ObjectTreeNode::ObjectTreeNode(ObjectTreeNode *left,
					   ObjectTreeNode *right) {
  object_ = NULL;
  left_ = left;
  right_ = right;
  min_ = min(left->min_, right->min_);
  max_ = max(left->max_, right->max_);
}

ObjectTree::ObjectTreeNode::~ObjectTreeNode() {
  destroy();
}

ObjectTree::ObjectTreeNode::ObjectTreeNode(ObjectTreeNode &other) {
  copy(other);
}

ObjectTree::ObjectTreeNode &
ObjectTree::ObjectTreeNode::operator=(ObjectTreeNode &other) {
  if (&other != this) {
    destroy();
    copy(other);
  }
  return *this;
}

void ObjectTree::ObjectTreeNode::copy(ObjectTreeNode &other) {
  object_ = other.object_;
  left_ = other.left_;
  right_ = other.right_;
}

void ObjectTree::ObjectTreeNode::destroy() {
  if (left_ != NULL) {
    delete left_;
  }
  left_ = NULL;
  if (right_ != NULL) {
    delete right_;
  }
  right_ = NULL;
}

Drawable *ObjectTree::ObjectTreeNode::findDrawable(Ray ray) {
  if (object_ == NULL) {
    // First check if ray intersects BV.
    
    // If false, then ray doesn't intersect anything inside
    if (!intersect(ray)) {
      return NULL;
    }
    // Neither the left nor right should be NULL
    Drawable *leftObject = left_->findDrawable(ray);
    // Guaranteed to be a positive intersection.
    if (leftObject == NULL) {
      return right_->findDrawable(ray);
    } else {
      Drawable *rightObject = right_->findDrawable(ray);
      if (rightObject == NULL) {
	return leftObject;
      }
      double leftDistance = leftObject->Intersects(ray);
      double rightDistance = rightObject->Intersects(ray);

      return leftDistance < rightDistance ? leftObject : rightObject;
    }
  }
  // Leaf node

  if (object_->Intersects(ray) < 0) {
    // No hit
    return NULL;
  }
  // Intersects
  return object_;
}

/**
 * Tests if the ray intersects the BV, not the object within
 */
bool ObjectTree::ObjectTreeNode::intersect(Ray ray) {
  // The ray is in the form o+td.
  // In each dimension, we solve for t by checking when that dimension
  //   is equal to the value of that dimension in min_ and max_.
  // Then, we compute the values of the other two dimension and check if
  //   they are within the values in min_ and max_
  // Work delegated to intersectPlane

  return intersectPlane(ray, ray.source.x_, ray.direction.x_, min_.x_) ||
    intersectPlane(ray, ray.source.x_, ray.direction.x_, max_.x_) ||
    intersectPlane(ray, ray.source.y_, ray.direction.y_, min_.y_) ||
    intersectPlane(ray, ray.source.y_, ray.direction.y_, max_.y_) ||
    intersectPlane(ray, ray.source.z_, ray.direction.z_, min_.z_) ||
    intersectPlane(ray, ray.source.z_, ray.direction.z_, max_.z_);
}

bool ObjectTree::ObjectTreeNode::intersectPlane(Ray ray, double ray_ori,
						double ray_dir,
						double target) {
  double t = -1; // t is how far the ray travels before hitting target
  // -1 because 0 means possibility of missing object at camera
  if (ray_dir != 0) {
    t = (target - ray_ori) / ray_dir;
  }
  if (t < 0) {
    return false;
  }
  double ERROR = -0.00005;
  Vector3 intersectPoint = ray.source + ray.direction * t;
  Vector3 distToMin = intersectPoint - min_;
  Vector3 distToMax = max_ - intersectPoint;
  return (distToMin.x_ >= ERROR && distToMin.y_ >= ERROR && distToMin.z_ >= ERROR &&
	  distToMax.x_ >= ERROR && distToMax.y_ >= ERROR && distToMax.z_ >= ERROR);
}

void ObjectTree::ObjectTreeNode::print(int height) {
  if (object_ != NULL) {
    for (int i = 0; i < 17 * (height); i ++) {
      std::cout << " ";
    }
    std::cout << "Object " << object_ << std::endl;
    return;
  }
  left_->print(height + 1);
  for (int i = 0; i < 17 * (height); i ++) {
    std::cout << " ";
  }
  std::cout << "Internal Node => ";
  std::cout << std::endl;
  right_->print(height + 1);

  return;
}
