// Geometry.h
// Copyright (c) 2011 James O'Doherty
// All Rights Reserved

#ifndef GEOMETRY_H__
#define GEOMETRY_H__

const float kPI = 3.1415927f;

struct Point
{
    float x;
    float y;
};

struct Circle
{
    Point center;
    float radius;
};

struct Rect
{
    float left;
    float top;
    float right;
    float bottom;
};


// Utility functions for geometry and collision detection

inline bool isOverlapping(Rect &r1, Rect &r2) {
    return (r1.left < r2.right &&
            r1.right > r2.left &&
            r1.bottom < r2.top && 
            r1.top > r2.bottom);
}

inline bool isOverlapping(Circle &p, Circle &q) {
    if ((p.radius+q.radius)*(p.radius+q.radius) > 
        (p.center.x - q.center.x)*(p.center.x - q.center.x) +
        (p.center.y - q.center.y)*(p.center.y - q.center.y))
        return true;
    return false;
}



#endif // GEOMETRY_H__
