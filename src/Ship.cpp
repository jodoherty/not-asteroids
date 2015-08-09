// Ship.cpp
// Copyright (c) 2011 James O'Doherty
// All Rights Reserved

#include <memory>

#include "Common.h"
#include "Ship.h"
#include "Bullet.h"
#include "Explosion.h"

using namespace std;

static const float size = 10.0f;

// Internal constants
const float Ship::thrust = 0.02f;
const float Ship::drag = 0.99f;
const int Ship::framecount = 3;
const float Ship::size = 0.5f*::size; 

static float vertices[] = {
    // ship body
    -size/2.0f+size/8.0f, -size/4.0f, 0.0f,
    size/2.0f+size/8.0f, 0.0f, 0.0f,
    -size/2.0f+size/8.0f, size/4.0f, 0.0f,
    // short tail
    -size/1.5f+size/8.0f, -size/5.0f, 0.0f,
    -size/2.0f+size/8.0f, 0.0f, 0.0f,
    -size/1.5f+size/8.0f, size/5.0f, 0.0f,
    // long tail
    -size+size/8.0f, -size/5.0f, 0.0f,
    -size/2.0f+size/8.0f, 0.0f, 0.0f,
    -size+size/8.0f, size/5.0f, 0.0f,
};

static GLuint frames[][6] = {{0, 1, 2, 0, 0, 0},
                             {0, 1, 2, 3, 4, 5},
                             {0, 1, 2, 6, 7, 8}};

Ship::Ship(float startx, float starty) 
{
    x = startx;
    y = starty;
    direction = kPI/2;
    clearState();
    blink = true;
    visible = true;
    step = 0;
}

Ship::~Ship() 
{
    #ifdef DEBUG
    std::cout << "Destroyed Ship!\n";
    #endif
}

void Ship::update()
{
    static float drift = 1.0f;
    const int framedelta = 4;
    if (alive) {
        if (left)
            direction += kPI/96.0f*drift;
        if (right)
            direction -= kPI/96.0f*drift;

        // Reset the turning speed if the user let goes of the keys
        if (!left && !right)
            drift = 1.0f;
        // Speed up turning as the user holds down the left or right key
        else if (drift < 2.0f)
            drift += 0.04f;
        if (accelerate) {
            dx += thrust * std::cos(direction);
            dy += thrust * std::sin(direction);
            if (step % framedelta == 0) 
                frame = (frame + 1) % framecount;
        }
        // Update dx and dy
        x += dx;
        y += dy;
        dx *= drag;
        dy *= drag;
    } else {
        explosion->update();
    }
    if (step % 5 == 0) {
        if (step < 120 && blink) {
            if (!visible)
                visible = true;
            else 
                visible = false;
        } else {
            visible = true;
            blink = false;
        }
    }
    step++;
}

void Ship::drawAt(float x, float y)
{
    if (!alive) {
        explosion->draw();
        return;
    }
    if (!visible)
        return;

    glPushMatrix();
    // Move to ship coordinates
    glTranslatef(x, y, 0.0f);
    glRotatef(direction*180/kPI, 0.0f, 0.0f, 1.0f);

    // Enable vertex arrays
    glEnableClientState(GL_VERTEX_ARRAY);

    // Load vertex pointer
    glVertexPointer(3, GL_FLOAT, 0, ::vertices);

    // Draw current frame
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ::frames[frame]);

    // Disable vertex arrays
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void Ship::drawIcon(float x, float y)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glScalef(0.6f, 0.6f, 1.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, ::vertices);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ::frames[0]);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

Bullet *Ship::fire()
{
    float dx = this->dx + 1.8f * cos(this->direction);
    float dy = this->dy + 1.8f * sin(this->direction);
    Bullet *bullet = new Bullet(this->x + size*cos(this->direction),
                                this->y + size*sin(this->direction),
                                dx, dy);
    this->dx -= dx*0.01f;
    this->dy -= dy*0.01f;
    return bullet;
}
