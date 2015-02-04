#ifndef MIRRORED_SPRITE_H__
#define MIRRORED_SPRITE_H__


#include "Sprite.h"

// MirroredSprite is an abstract class that implements default member
// functions that automatically mirror the sprite whenever it reaches
// the edge of the playing field.
class MirroredSprite : public Sprite
{
public:
    // Additional required accessors
    virtual void setLocation(Point) = 0;

    // Implementation for subclasses to inherit
    void draw() {
        float size = getSize();
        Point location = getLocation();
        float &x = location.x;
        float &y = location.y;
        // Draw the sprite at the actual location
        drawAt(x, y);

        // Now determine if mirrored images at the opposite sides
        // of the screen are needed
        bool horizontal = true;
        bool vertical = true;
        float mx = x;
        float my = y;
        if (x+size > 100.0f) 
            mx = -200.0f + x;
        else if (x-size < -100.0f)
            mx = 200.0f + x;
        else
            horizontal = false;
        if (y+size > 100.0f )
            my = -200.0f + y;
        else if (y-size < -100.0f)
            my = 200.0f + y;
        else
            vertical = false;
        if (horizontal && vertical) 
            drawAt(mx, my);
        if (horizontal) 
            drawAt(mx, y);
        if (vertical)
            drawAt(x, my);
    }

    // Ensure collision detection accounts for the fact that
    // our object might be mirrored at 3 different points of
    // the screen.
    bool detectCollision(Sprite &s) {
        if (!isPhysical() || !s.isPhysical())
            return false;

        Circle p = {getLocation(), getSize()/2.0f};
        Circle q = {s.getLocation(), s.getSize()/2.0f};

        if (isOverlapping(p, q))
            return true;

        // Now determine if mirrored copies of the object on the other side
        // of the screen are needed and if they collide with our target
        float &size = p.radius;
        float x = p.center.x;
        float y = p.center.y;

        bool horizontal = true;
        bool vertical = true;
        float mx = x;
        float my = y;
        if (x+size > 100.0f) 
            mx = -200.0f + x;
        else if (x-size < -100.0f)
            mx = 200.0f + x;
        else
            horizontal = false;
        if (y+size > 100.0f )
            my = -200.0f + y;
        else if (y-size < -100.0f)
            my = 200.0f + y;
        else
            vertical = false;
        if (horizontal && vertical) {
            p.center.x = mx;
            p.center.y = my;
            if (isOverlapping(p, q))
                return true;
        }
        if (horizontal) {
            p.center.x = mx;
            p.center.y = y;
            if (isOverlapping(p, q))
                return true;
        }
        if (vertical) {
            p.center.x = x;
            p.center.y = my;
            if (isOverlapping(p, q))
                return true;
        }
        return false;
    }
    // Force mirrored sprites to wrap around the screen
    void postUpdateHandler() {
        Point p = getLocation();
        if (p.x > 100.0f)
            p.x += -200.0f;
        else if (p.x < -100.0f)
            p.x += 200.0f;
        if (p.y > 100.0f)
            p.y += -200.0f;
        else if (p.y < -100.0f)
            p.y += 200.0f;
        setLocation(p);
    }

    bool isOutOfBounds() {
        return false;
    }
};

#endif // MIRRORED_SPRITE_H__
