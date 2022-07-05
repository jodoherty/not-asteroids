// Copyright (c) 2011 James O'Doherty
// All Rights Reserved.

#include "Asteroid.h"

#include <algorithm>
#include <ctime>

#include "Common.h"

using namespace std;

const float Asteroid::kMaxSize = 64.0f;
const float Asteroid::kMinSize = 7.0f;

static const GLfloat vertices[] = {-1.0f,   -0.667f, 0.0f,   -1.0f, 0.667f,
                                   -0.667f, 0.5f,    -0.5f,  1.0f,  0.0f,
                                   0.667f,  0.667f,  0.0f,   1.0f,  -0.667f,
                                   0.667f,  -0.667f, 0.333f, -1.0f, 0.0f};

static const GLuint indices[] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5,
                                 5, 6, 6, 7, 7, 8, 8, 9, 9, 0};
static const int indice_count = 20;

void Asteroid::init() {
  angle = kPI / 4;
  angular_velocity = kPI / 64.0f / (rand() % 3 + 1);
  if (size > kMaxSize)
    size = kMaxSize;
  else if (size < kMinSize)
    size = kMinSize;
  disabled = false;
}

Asteroid::Asteroid(float size) : size(size) {
  float a = (rand() % 100) / 50.0f * kPI;
  x = 99.0f * cos(a);
  y = 99.0f * sin(a);
  init();
  Vector2d p(-x, -y);
  p.normalize();
  velocity = p * 0.3f;
}

Asteroid::Asteroid(float x, float y, float size) : x(x), y(y), size(size) {
  init();
  float heading = kPI * static_cast<float>(rand() % 100) / 50.0f;
  float speed =
      static_cast<float>(rand() % 55) / 50.0f * (size / (size + 0.5f)) * 0.3f;
  velocity.x = speed * cos(heading);
  velocity.y = speed * sin(heading);
}

Asteroid::Asteroid(float x, float y, float size, Point center)
    : x(x), y(y), size(size) {
  init();
  float dy = y - center.y;
  float dx = x - center.x;
  float heading = atan(dy / dx);
  float speed =
      static_cast<float>(rand() % 55) / 50.0f * (size / (size + 0.5f)) * 0.3f;
  velocity.x = speed * cos(heading);
  velocity.y = speed * sin(heading);
}

Asteroid::~Asteroid() {
#ifdef DEBUG
  cout << "Destroyed Asteroid!\n";
#endif
}

bool Asteroid::createChildren(Asteroid *children[]) {
  if (disabled || size / 2.0f < kMinSize) return false;
  children[0] = new Asteroid(x - size / 2.5f, y, size / 2.0f);
  children[1] = new Asteroid(x, y - size / 2.5f, size / 2.0f);
  children[2] = new Asteroid(x + size / 2.5f, y, size / 2.0f);
  children[3] = new Asteroid(x, y + size / 2.5f, size / 2.0f);
  return true;
}

void Asteroid::collide(Asteroid &a) {
  float m1 = size * size;
  float m2 = a.size * a.size;
  Vector2d normal(a.velocity.x - velocity.x, a.velocity.y - velocity.y);
  normal.normalize();
  Vector2d tangent(-normal.y, normal.x);
  float v1n = normal * velocity;
  float v1t = tangent * velocity;
  float v2n = normal * a.velocity;
  float v2t = tangent * a.velocity;
  float v1n_ = (v1n * (m1 - m2) + 2 * m2 * v2n) / (m1 + m2);
  float v2n_ = (v2n * (m2 - m1) + 2 * m1 * v1n) / (m1 + m2);
  velocity = v1n_ * normal + v1t * tangent;
  a.velocity = v2n_ * normal + v2t * tangent;
}

void Asteroid::update() {
  angle += angular_velocity;
  x += velocity.x;
  y += velocity.y;
}

void Asteroid::drawAt(float x, float y) {
  glPushMatrix();
  glTranslatef(x, y, 0.0f);
  glScalef(size / 2.0f, size / 2.0f, 1.0f);
  glRotatef(angle * 180 / kPI, 0.0f, 0.0f, 1.0f);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glDrawElements(GL_LINES, indice_count, GL_UNSIGNED_INT, indices);
  glDisableClientState(GL_VERTEX_ARRAY);
  glPopMatrix();
}
