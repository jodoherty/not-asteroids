// Bullet.h
// Copyright (c) 2011 James O'Doherty
// All Rights Reserved.

#ifndef BULLET_H__
#define BULLET_H__

#include "MirroredSprite.h"

class Bullet : public Sprite {
  float x;
  float y;
  float dx;
  float dy;
  static const float size;

 public:
  Bullet(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy) {}

  ~Bullet() {
#ifdef DEBUG
    std::cout << "Destroyed Bullet!\n";
#endif
  }

  float getSize() { return size; }
  Point getLocation() {
    Point p = {x, y};
    return p;
  }
  void setLocation(Point p) {
    x = p.x;
    y = p.y;
  }

  void update();
  void drawAt(float x, float y);

  friend class Ship;
};

#endif  // BULLET_H__
