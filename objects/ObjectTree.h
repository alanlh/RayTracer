#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <vector>

#include "Drawable.h"
#include "../scene/Ray.h"
#include "../scene/Vector3.h"

class ObjectTree {
 public:
  
  class ObjectTreeNode {
  public:
    /**
     * Constructs a BV around an object. 
     * For leaf nodes.
     */
    ObjectTreeNode(Drawable *object);
    /**
     * Constructs a BV around two smaller BVs
     * For internal nodes.
     */
    ObjectTreeNode(ObjectTreeNode *left, ObjectTreeNode *right);
    /**
     * Destructor. Calles delete on children but NOT object
     */
    ~ObjectTreeNode();
    
    ObjectTreeNode(ObjectTreeNode &other);
    ObjectTreeNode & operator=(ObjectTreeNode &other);
    
    void copy(ObjectTreeNode &other);
    void destroy();

    /**
     * Tests if a ray intersects an object w/in the BV
     * If internal node, continues to test children if does intersect.
     * If both children not NULL, then calls intersect on both to find the 
     * minimum distance. If one NULL, then return.
     * Otherwise returns NULL
     * If leaf node, calls object_'s intersect function. If intersects, return
     * pointer. Otherwise, return NULL. 
     */
    Drawable *findDrawable(Ray ray);
    /**
     * Tests if the ray intersects the BV.
     */ 
    bool intersect(Ray ray);
    /**
     * Helper function for intersect. Does the actual testing
     */
    bool intersectPlane(Ray ray, double ray_ori, double ray_dir, double target);

    /** 
     * Prints information regarding the object and recursively calls print
     * on children, if any
     */
    void print(int height);
    
    Drawable *object_;
    Vector3 min_;
    Vector3 max_;
    ObjectTreeNode *left_;
    ObjectTreeNode *right_;
  };
  

  /**
   * Constructs a BVH from a list of object pointers.
   */
  ObjectTree(std::vector<Drawable *> & objects);
  /**
   * Destructor. Calls delete on root_
   */
  ~ObjectTree();

  /**
   * Helper function for the constructor
   * Partitions a region of the vector objects, and constructs a subtree
   */
  ObjectTreeNode *construct(std::vector<Drawable *> &objects, int lo, int hi,
			    int axis);
  /**
   * Partitions an array of Drawable pointers from index lo to hi, inclusive
   * Along a certain axis
   * Bottom half smaller than upper half. 
   */
  void partition(std::vector<Drawable *> &objects, int lo, int hi, int axis);
  
  ObjectTree(ObjectTree &other);
  ObjectTree & operator=(ObjectTree &other);

  /**
   * Calls intersect on the root, and returns the closest object to be rendered
   */
  Drawable *intersect(Ray ray);
  void print();
 private:
   
  void copy(ObjectTree &other);
  void destroy();

  /**
   * Swaps two elements in the vector. 
   */
  void swap(std::vector<Drawable *> &objects, int lo, int hi);
  
  ObjectTreeNode *root_;
};

#endif
