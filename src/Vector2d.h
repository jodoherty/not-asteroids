#ifndef VECTOR_2D_H__
#define VECTOR_2D_H__

#include <cmath>

class Vector2d {
 public:
  float x;
  float y;

  Vector2d() : x(0.0f), y(0.0f) {}

  Vector2d(float x, float y) : x(x), y(y) {}

  float getMagnitude();
  void normalize();
  void reverse();

  Vector2d &operator+=(Vector2d);
  Vector2d &operator-=(Vector2d);
  Vector2d &operator*=(float);
  Vector2d &operator/=(float);

  Vector2d operator-();
};

inline float Vector2d::getMagnitude() {
  return (float)std::sqrt(x * x + y * y);
}

inline void Vector2d::normalize() {
  const float tol = 0.0001f;
  float m = getMagnitude();
  if (m <= tol) m = 1;
  x /= m;
  y /= m;

  if (std::fabs(x) < tol) x = 0.0f;
  if (std::fabs(y) < tol) y = 0.0f;
}

inline void Vector2d::reverse() {
  x = -x;
  y = -y;
}

inline Vector2d &Vector2d::operator+=(Vector2d u) {
  x += u.x;
  y += u.y;
  return *this;
}

inline Vector2d &Vector2d::operator-=(Vector2d u) {
  x -= u.x;
  y -= u.y;
  return *this;
}

inline Vector2d &Vector2d::operator*=(float s) {
  x *= s;
  y *= s;
  return *this;
}

inline Vector2d &Vector2d::operator/=(float s) {
  x /= s;
  y /= s;
  return *this;
}

inline Vector2d Vector2d::operator-() { return Vector2d(-x, -y); }

inline Vector2d operator+(Vector2d u, Vector2d v) {
  return Vector2d(u.x + v.x, u.y + v.y);
}

inline Vector2d operator-(Vector2d u, Vector2d v) {
  return Vector2d(u.x - v.x, u.y - v.y);
}

inline float operator*(Vector2d u, Vector2d v) {
  return (u.x * v.x + u.y * v.y);
}

inline Vector2d operator*(float s, Vector2d u) {
  Vector2d p(u.x * s, u.y * s);
  return p;
}

inline Vector2d operator*(Vector2d u, float s) {
  Vector2d p(u.x * s, u.y * s);
  return p;
}

inline Vector2d operator/(Vector2d u, float s) {
  Vector2d p(u.x / s, u.y / s);
  return p;
}

#endif  // VECTOR_2D_H__
