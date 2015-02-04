// Ship.h
// Copyright (c) 2011 James O'Doherty
// All Rights Reserved.

#ifndef SHIP_H__
#define SHIP_H__

#include "MirroredSprite.h"
#include "Explosion.h"
#include "Bullet.h"

class Ship : public MirroredSprite
{
    float x;
    float y;
    float direction;
    float dx;
    float dy;
    bool left;
    bool right;
    bool accelerate;
    int frame;
    unsigned int step;
    bool alive;
    bool blink;
    bool visible;
    Explosion *explosion;
    static const int framecount;
    static const float thrust;
    static const float drag;
    static const float size;

public:
    Ship(float = 0.0, float = 0.0);
    ~Ship();

    // Required accessors
    float getSize() {
        return size;
    }
    Point getLocation() {
        Point p = {x,y};
        return p;
    }
    void setLocation(Point p) {
        x = p.x;
        y = p.y;
    }

    // Optional overrides
    bool isPhysical() {
        return !blink;
    }
    bool isVisible() {
        return visible;
    }

    // Other accessors
    bool isAlive() {
        return alive;
    }
    bool isReady() {
        return explosion->isFinished();
    }
    bool revive() {
        if (isReady()) {
            dx = 0;
            dy = 0;
            alive = true;
            step = 0;
            blink = true;
            delete explosion;
            return true;
        }
        return false;
    }
    void kill() {
        alive = false;
        frame = 0;
        explosion = new Explosion(x,y);
    }

    // Used for player controls
    void setLeft() {
        left = true;
    }
    void clearLeft() {
        left = false;
    }
    void setRight() {
        right = true;
    }
    void clearRight() {
        right = false;
    }
    void setAccelerate() {
        accelerate = true;
    }
    void clearAccelerate() {
        frame = 0;
        accelerate = false;
    }

    void drawAt(float x, float y);
    void update();

    void drawIcon(float x, float y);

    Bullet *fire();

private:
    void clearState() {
        left = false;
        right = false;
        accelerate = false;
        dx = 0.0;
        dy = 0.0;
        frame = 0;
        alive = true;
    }
};

#endif // SHIP_H__
