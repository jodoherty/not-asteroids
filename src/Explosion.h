#ifndef EXPLOSION_H__
#define EXPLOSION_H__

#include <cmath>
#include <cstdlib>

#include "Common.h"
#include "Sprite.h"
#include "Vector2d.h"

class Explosion : public Sprite {
  float x;
  float y;
  float points[200];
  float points2[200];
  float points3[200];
  Vector2d velocities[100];
  float color[4];
  float psize;
  unsigned long int step;

 public:
  Explosion(float x, float y) : x(x), y(y) {
    for (int i = 0; i < 200; i++) {
      points[i] = 0.0;
      points2[i] = 0.0;
      points3[i] = 0.0;
    }
    for (int i = 0; i < 100; i++) {
      velocities[i].x = 1.0f * cos(i * kPI / 50.0f) + (std::rand() % 2) / 20.0f;
      velocities[i].y = 1.0f * sin(i * kPI / 50.0f) + (std::rand() % 2) / 20.0f;
    }
    color[0] = 1.0f;
    color[1] = 0.8f;
    color[2] = 0.4f;
    color[3] = 1.0f;
    step = 0;
    psize = 3.0f;
  }
  ~Explosion() {}

  Point getLocation() {
    Point p = {x, y};
    return p;
  }

  float getSize() { return 10.0f; }

  bool isPhysical() { return false; }

  void drawAt(float x, float y) {
    glPushMatrix();
    // Enable vertex arrays
    glEnableClientState(GL_VERTEX_ARRAY);

    glTranslatef(x, y, 0.0f);
    glPushAttrib(GL_CURRENT_BIT);
    glColor4fv(color);
    glPointSize(psize);
    glVertexPointer(2, GL_FLOAT, 0, points);
    glDrawArrays(GL_POINTS, 0, 100);
    glVertexPointer(2, GL_FLOAT, 0, points2);
    glDrawArrays(GL_POINTS, 0, 100);
    glVertexPointer(2, GL_FLOAT, 0, points3);
    glDrawArrays(GL_POINTS, 0, 100);
    glPopAttrib();

    // Disable vertex arrays
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
  }

  void update() {
    for (int i = 0; i < 100; i++) {
      points[2 * i] += velocities[i].x;
      points[2 * i + 1] += velocities[i].y;
      points2[2 * i] += velocities[i].x / 2.0f;
      points2[2 * i + 1] += velocities[i].y / 2.0f;
      points3[2 * i] += velocities[i].x / 3.0f;
      points3[2 * i + 1] += velocities[i].y / 3.0f;
      velocities[i].x *= 0.94f;
      velocities[i].y *= 0.94f;
    }
    color[3] *= 0.90f;
    psize *= 0.98f;
  }

  bool isFinished() {
    if (psize < 1.0f) return true;
    return false;
  }
};

#endif  // EXPLOSION_H__
