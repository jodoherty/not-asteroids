// Bullet.cpp
// Copyright (c) 2011 James O'Doherty
// All Rights Reserved.

#include "Common.h"
#include "Bullet.h"

const float Bullet::size = 1.2f;

void Bullet::update() 
{
    x += dx;
    y += dy;
}

void Bullet::drawAt(float x, float y)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRectf(-size/2.0f, -size/2.0f, size/2.0f, size/2.0f);
    glPopMatrix();
}

