#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <vector>

#include "Drawable.h"
#include "../scene/Ray.h"

class ObjectTree {
 public:
  ObjectTree(std::vector<Drawable *> & objects);
  ~ObjectTree();

  ObjectTreeNode *construct(std::vector<Drawable *> &objects, int lo, int hi,
			    int axis);
  void partition(std::vector<Drawable *> &objects, int lo, int hi, int axis);
  
  ObjectTree(ObjectTree &other);
  ObjectTree & operator=(ObjectTree &other);
  
  void convertToTree();

  double intersect(Ray ray);
  
 private:
   
  void copy(ObjectTree &other);
  void destroy();

  void swap(std::vector<Drawable *> &objects, int lo, int hi);
  
  ObjectTreeNode *root_;

  class ObjectTreeNode {
  public:
    ObjectTreeNode(Drawable *objects);
    ObjectTreeNode(ObjectTreeNode *left, ObjectTreeNode *right);
    ~ObjectTreeNode();
    
    ObjectTreeNode(ObjectTreeNode &other);
    ObjectTreeNode & operator=(ObjectTreeNode &other);
    
    void copy(ObjectTreeNode &other);
    void destroy();

    double intersect(Ray ray);
    
    Drawable *object_;
    Vector3 min_;
    Vector3 max_;
    ObjectTreeNode *left_;
    ObjectTreeNode *right_;
  };

};

#endif
