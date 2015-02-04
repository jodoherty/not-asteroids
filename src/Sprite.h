// Sprite.h
// Copyright (c) 2011 James O'Doherty
// All Rights Reserved

#ifndef SPRITE_H__
#define SPRITE_H__

#include <iostream>
#include "Geometry.h" 

// Sprite is an abstract class describing an interface for an object that 
// maintains a modeled state and can render itself onto the OpenGL screen.
class Sprite
{
public:
    virtual ~Sprite() { 
        #ifdef DEBUG
        std::cout << "Destroyed Sprite!\n"; 
        #endif
    };
    // Required accessors
    virtual float getSize() = 0;
    virtual Point getLocation() = 0;

    // Required game loop calls
    virtual void update() = 0;
    virtual void drawAt(float, float) = 0;

    // Default drawing routine
    virtual void draw() {
        Point p = getLocation();
        drawAt(p.x, p.y);
    }

    // Optional hooks for adding handlers that get
    // called before and after update() respectively
    virtual void preUpdateHandler() {}
    virtual void postUpdateHandler() {}

    // Optional collision detection use override
    virtual bool isPhysical() {
        return true;
    }
    virtual bool isVisible() {
        return true;
    }

    // Default collision detection routine
    virtual bool detectCollision(Sprite &s) {
        if (!isPhysical() || !s.isPhysical())
            return false;

        Circle p = {getLocation(), getSize()/2.0f};
        Circle q = {s.getLocation(), s.getSize()/2.0f};

        if (isOverlapping(p, q))
            return true;
        return false;
    }

    virtual bool isOutOfBounds() {
        Point p = getLocation();
        return (p.x < -101.0f ||
                p.y < -101.0f ||
                p.x > 101.0f ||
                p.y > 101.0f);
    }

};


#endif // SPRITE_H__
