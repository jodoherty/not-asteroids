// Copyright (c) 2011 James O'Doherty
// All Rights Reserved.

#ifndef ASTEROID_H__
#define ASTEROID_H__

#include "MirroredSprite.h"
#include "Vector2d.h"

class Asteroid : public MirroredSprite
{
    float x;
    float y;
    Vector2d velocity;
    float size;
    float angle;
    float angular_velocity;
    bool disabled;

    void init();

public:
    Asteroid(float size);
    Asteroid(float x, float y, float size);
    Asteroid(float x, float y, float size, Point center);
    ~Asteroid();

    // Child factory function
    bool createChildren(Asteroid **);

    // Required accessors
    float getSize() {
        return size;
    }
    Point getLocation() {
        Point p = {x, y};
        return p;
    }
    void setLocation(Point p) {
        x = p.x;
        y = p.y;
    }

    // Asteroid specific accessors
    void disable() {
        disabled = true;
    }

    void reverse() {
        x -= 0.1f*velocity.x;
        y -= 0.1f*velocity.y;
    }

    // collide handles collisions with other Asteroids
    void collide(Asteroid &);

    void update();
    void drawAt(float x, float y);

    static const float kMaxSize;
    static const float kMinSize;

};

#endif // ASTEROID_H__
