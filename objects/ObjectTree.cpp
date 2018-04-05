#include "ObjectTree.h"

#include <vector>

#include "../scene/Ray.h"
#include "Drawable.h"

ObjectTree::ObjectTree(std::vector<Drawable *> & objects) {
  root_ = NULL;
  if (objects.size() == 0) {
    return;
  }
  if (objects.size() == 1) {
    root_ = new ObjectTreeNode(objects.[0]);
    return;
  }

  root_ = construct(objects, 0, objects.size() - 1, 0);
}

ObjectTreeNode *ObjectTree::construct(std::vector<Drawable *> &objects,
				      int lo, int hi, int axis) {
  if (lo == hi) {
    return new ObjectTreeNode(objects[lo]);
  }
  partition(objects, lo, hi, axis);
  ObjectTreeNode *left = construct(objects, lo, (lo + hi) / 2, (axis + 1) % 3);
  ObjectTreeNode *right = construct(objects, (lo + hi) / 2 + 1, hi,
				    (axis + 1) % 3);
  
  return *parent = new ObjectTreeNode(left, right);
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
	swap(points, lower, higher);
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
}

void ObjectTree::copy(ObjectTree &other) {
  *root_ = *other.root;
  for (Drawable *object : other.objects_) {
    objects_.push_back(object)
  }
}

void ObjectTree::destroy() {
  if (root_ != NULL) {
    delete root;
  }
}

ObjectTree::ObjectTreeNode::ObjectTreeNode(Drawable *object) {
  object_ = object;
  left_ = NULL;
  right = NULL;
  min_ = object->min();
  max_ = object->max();
}

ObjectTree::ObjectTreeNode::ObjectTreeNode(ObjectTreeNode *left,
					   ObjectTreeNode *right) {
  object_ = NULL;
  left_ = left;
  right_ = right;
  min_ = Vector3::min(left->min_, right->min_);
  max_ = Vector3::max(left->max_, right->max_);
}

ObjectTree::ObjectTreeNode::~ObjectTreeNode() {
  destroy();
}

ObjectTree::ObjectTreeNode::ObjectTree(ObjectTreeNode &other) {
  copy(other);
}

ObjectTreeNode &
ObjectTree::ObjectTreeNode::operator=(ObjectTreeNode &other) {
  if (&other != this) {
    destroy();
    copy(other);
  }
}

void ObjectTree::ObjectTreeNode::copy(ObjectTreeNode &other) {
  object_ = other.object_;
  left_ = other.left_;
  right_ = other.right_;
  is_leaf = other.is_leaf;
  height = other.height;
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

