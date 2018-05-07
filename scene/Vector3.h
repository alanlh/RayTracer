#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3 {
 public:
  Vector3();
  Vector3(double x, double y, double z);
  //Vector3(Vector3 &other);
  ~Vector3();

  void operator=(Vector3 other);
  Vector3 operator+(Vector3 other);
  Vector3 operator*(double scale);
  Vector3 operator-(Vector3 other);

  double magnitude();
  bool isUnit();
  void makeUnitVector();

  /**
   * Creates and returns a vector perpendicular to the current vector.
   * Points towards the negative-Z axis.
   */
  void pointTowardsZPos();
  void pointTowardsYPos();

  void rotate(double angleDeg, Vector3 axis);

  void print();

  double x_;
  double y_;
  double z_;
};

double distance(Vector3 first, Vector3 second);
double dotProduct(Vector3 first, Vector3 second);
Vector3 crossProduct(Vector3 first, Vector3 second);
Vector3 min(Vector3 first, Vector3 second);
Vector3 max(Vector3 first, Vector3 second);

#endif
