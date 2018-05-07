#include "Vector3.h"

#include <iostream>
#include "math.h"

Vector3::Vector3() : x_(0), y_(0), z_(0) {
  
}

Vector3::Vector3(double x, double y, double z) : x_(x), y_(y), z_(z) {

}

// Vector3::Vector3(Vector3 &other) {
//   x_ = other.x_;
//   y_ = other.y_;
//   z_ = other.z_;
// }

Vector3::~Vector3() {

}

void Vector3::operator=(Vector3 other) {
  x_ = other.x_;
  y_ = other.y_;
  z_ = other.z_;
}

Vector3 Vector3::operator+(Vector3 other) {
  return Vector3((x_ + other.x_), (y_ + other.y_), (z_ + other.z_));
}

Vector3 Vector3::operator*(double scale) {
  return Vector3(x_ * scale, y_ * scale, z_ * scale);
}

Vector3 Vector3::operator-(Vector3 other) {
  return Vector3((x_ - other.x_), (y_ - other.y_), (z_ - other.z_));
}

double Vector3::magnitude() {
  return sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2));
}

bool Vector3::isUnit() {
  double maxError = 0.00000001;
  return (pow(x_, 2) + pow(y_, 2) + pow(z_, 2) - 1 < maxError);
}

void Vector3::makeUnitVector() {
  double magnitude = sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2));
  double scale = 1 / magnitude;
  
  x_ *= scale;
  y_ *= scale;
  z_ *= scale;
}

/* Picks the normal vector that maximizes the z-value. 
 * If normal vector is perpendicular to z-axis, then points in +y direction.
 * TODO: Make it so that when z_ ==0, point towards tilt angle
 */
void Vector3::pointTowardsZPos() {
  double magnitude = this->magnitude();
  if (fabs(z_) < 0.00000001) {
    x_ = 0;
    y_ = 0;
    z_ = fabs(magnitude);
    return;
  }
  if (fabs(x_) < 0.00000001 && fabs(y_) < 0.00000001) {
    x_ = fabs(z_);
    y_ = 0;
    z_ = 0;
    return;
  }
  
  double new_x = -1 * z_ * x_ * z_ * z_ / (fabs(z_) * (x_ * x_ + y_ * y_));
  double new_y = -1 * z_ * y_ * z_ * z_ / (fabs(z_) * (x_ * x_ + y_ * y_)); 
  double new_z = fabs(z_);
  x_ = new_x;
  y_ = new_y;
  z_ = new_z;
  this->makeUnitVector();
  *this = (*this) * magnitude;
}

/* Picks the normal vector that maximizes the y-value. 
 * If normal vector is perpendicular to y-axis, then points in +z direction.
 * TODO: Make it so that when y_ ==0, point towards tilt angle
 */
void Vector3::pointTowardsYPos() {
  double magnitude = this->magnitude();
  if (fabs(y_) < 0.00000001) {
    x_ = 0;
    y_ = 0;
    y_ = fabs(magnitude);
    return;
  }
  if (fabs(z_) < 0.00000001 && fabs(x_) < 0.00000001) {
    x_ = fabs(y_);
    y_ = 0;
    z_ = 0;
    return;
  }
  
  double new_x = -1 * y_ * y_ * y_ * y_ / (fabs(y_) * (x_ * x_ + z_ * z_));
  double new_z = -1 * y_ * z_ * y_ * y_ / (fabs(y_) * (x_ * x_ + z_ * z_)); 
  double new_y = fabs(y_);
  x_ = new_x;
  y_ = new_y;
  z_ = new_z;
  this->makeUnitVector();
  *this = (*this) * magnitude;
}


/**
 * Rotates the vector angleDeg degrees clockwise about the given axis
 */
void Vector3::rotate(double angleDeg, Vector3 axis) {  
  double magnitude = this->magnitude();
  double angleRad = angleDeg * 3.141592965 / 180;
  double new_x = (1 - cos(angleRad)) * axis.x_
    * (x_ * axis.x_ + y_ * axis.y_ + z_ * axis.z_) + x_ * cos(angleRad)
    + sin(angleRad) * (axis.y_ * z_ - axis.z_ * y_);
  double new_y = (1 - cos(angleRad)) * axis.y_
    * (x_ * axis.x_ + y_ * axis.y_ + z_ * axis.z_) + y_ * cos(angleRad)
    + sin(angleRad) * (axis.z_ * x_ - axis.x_ * z_);
  double new_z = (1 - cos(angleRad)) * axis.z_
    * (x_ * axis.x_ + y_ * axis.y_ + z_ * axis.z_) + z_ * cos(angleRad)
    + sin(angleRad) * (axis.x_ * y_ - axis.y_ * x_);
  x_ = new_x;
  y_ = new_y;
  z_ = new_z;
  this->makeUnitVector();
  x_ *= magnitude;
  y_ *= magnitude;
  z_ *= magnitude;
}

void Vector3::print() {
  std::cout << "(" << x_ << ", " << y_ << ", " << z_ << ")" << std::endl;
}

double distance(Vector3 first, Vector3 second) {
  return sqrt(pow((first.x_ - second.x_), 2)
	      + pow((first.y_ + second.y_), 2)
	      + pow((first.z_ + second.z_), 2));
}

double dotProduct(Vector3 first, Vector3 second) {
  return first.x_*second.x_ + first.y_*second.y_ + first.z_*second.z_;
}

Vector3 crossProduct(Vector3 first, Vector3 second) {
  return Vector3(first.y_ * second.z_ - first.z_ * second.y_,
		 first.z_ * second.x_ - first.x_ * second.z_,
		 first.x_ * second.y_ - first.y_ * second.x_);
}

Vector3 min(Vector3 first, Vector3 second) {
  double x = first.x_ < second.x_ ? first.x_ : second.x_;
  double y = first.y_ < second.y_ ? first.y_ : second.y_;
  double z = first.z_ < second.z_ ? first.z_ : second.z_;

  return Vector3(x, y, z);
}

Vector3 max(Vector3 first, Vector3 second) {
  double x = first.x_ > second.x_ ? first.x_ : second.x_;
  double y = first.y_ > second.y_ ? first.y_ : second.y_;
  double z = first.z_ > second.z_ ? first.z_ : second.z_;

  return Vector3(x, y, z);
}
