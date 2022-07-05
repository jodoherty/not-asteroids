// Game.cpp
// Copyright (c) 2011 James O'Doherty
// All Rights Reserved.

#include <SDL.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "Common.h"

static SDL_Surface *screen = NULL;

#include "Asteroid.h"
#include "BitmapFont.h"
#include "Bullet.h"
#include "Game.h"
#include "Ship.h"

using namespace std;
using namespace std::placeholders;

Game::Game() {
  int width, height;
  // Initialize SDL subsystems
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
    cerr << "Error, couldn't initialize SDL: " << SDL_GetError() << endl;
    exit(EXIT_FAILURE);
  }
  atexit(SDL_Quit);

  // Initialize SDL screen
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  screen =
      SDL_SetVideoMode(800, 600, 0, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE);

  if (!screen) {
    cerr << "Error, video initialization failed: " << SDL_GetError() << endl;
    exit(EXIT_FAILURE);
  }

  width = screen->w;
  height = screen->h;

  // Initialize GLEW
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    cerr << "Error, glewInit failed: " << glewGetErrorString(err) << endl;
    exit(EXIT_FAILURE);
  }

  // Set up the screen
  reshape(width, height);

  // Set up our audio service
  audio = GetAudioServiceInstance();

  // Initialize sprites and scene data list
  shared_ptr<Ship> splayer(new Ship());
  player = splayer;
  startGame();
}

Game::~Game() { audio->release(); }

void Game::run() {
  srand(static_cast<unsigned int>(time(NULL)));

  // Start playing background music
  audio->playTrack(r_bg_music_track);

  // Prepare game loop
  bool quit = false;
  Uint32 ticks = SDL_GetTicks();
  const Uint32 delta = 20;

  // Run game loop
  while (!quit) {
    quit = poll();
    // Update game physics if the correct time delta has passed
    while (SDL_GetTicks() > ticks + delta) {
      update();
      ticks += delta;
    }
    // Render screen and sprites
    draw();
    // Detect and report OpenGL error conditions
    while (GLenum error = glGetError() != GL_NO_ERROR) {
      cerr << gluErrorString(error) << endl;
    }
  }
}

void Game::toggleFullScreen() {
  // *todo*
}

void Game::reshape(int w, int h) {
  if (h == 0) h = 1;
  GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;

  // Set up the OpenGL project screen
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Fill the entire window
  glViewport(0, 0, w, h);
  // Center the projection on 0,0 and ensure an x,y
  // range of at least 100 in all 4 directions is visible.
  if (w <= h) {
    left = w / 22;
    bottom = (h - w) / 2 + w / 22;
    width = w - w / 11;
    height = w - w / 11;
    glOrtho(-110.0, 110.0, -110.0 / aspectRatio, 110.0 / aspectRatio, 0, 1);
  } else {
    bottom = h / 22;
    left = (w - h) / 2 + h / 22;
    width = h - h / 11;
    height = h - h / 11;
    glOrtho(-110.0 * aspectRatio, 110.0 * aspectRatio, -110.0, 110.0, 0, 1);
  }

  glMatrixMode(GL_MODELVIEW);
  glDisable(GL_DEPTH_TEST);
  glLoadIdentity();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Game::update() {
  static int bonus = 1;

  vector<shared_ptr<Sprite>>::iterator it;
  vector<shared_ptr<Sprite>>::iterator it2;

  vector<shared_ptr<Sprite>>::iterator list_it;
  vector<shared_ptr<Sprite>> add_list;

  if (score > bonus * 4000) {
    lives++;
    bonus++;
  }
  if (player->isAlive() && sprites.size() < 2) {
    while (add_list.size() < 5) {
      shared_ptr<Sprite> a(new Asteroid(rand() % 8 + 8.0f + level));
      add_list.push_back(a);
    }
    level++;
  }

  // remove any objects that have moved out of bounds
  sprites.erase(remove_if(sprites.begin(), sprites.end(),
                          bind(&Sprite::isOutOfBounds, _1)),
                sprites.end());
  // Call update handlers
  for_each(sprites.begin(), sprites.end(), bind(&Sprite::preUpdateHandler, _1));
  for_each(sprites.begin(), sprites.end(), bind(&Sprite::update, _1));
  for_each(sprites.begin(), sprites.end(),
           bind(&Sprite::postUpdateHandler, _1));

  // Run collision detection and game physics
  it = sprites.begin();
  while (it != sprites.end()) {
    it2 = it;
    it2++;
    while (it2 != sprites.end()) {
      if ((*it)->detectCollision(**it2)) {
        // Use runtime type information to determine the appropriate
        // course of action for the sprites.

        // A more flexible method would be to create a SpriteManager
        // class that all sprites have a delegate to and require
        // Sprite class descendents implement a collision event
        // handler. Then every class would have everything it
        // needs to handle events.

        // I haven't gotten around to doing that yet.

        // Do we have an asteroid hitting an asteroid?
        Asteroid *a1 = dynamic_cast<Asteroid *>(it->get());
        Asteroid *a2 = dynamic_cast<Asteroid *>(it2->get());

        if (a1 && a2) {
          // Back up the asteroids so they don't overlap
          while ((*it)->detectCollision(**it2)) {
            a1->reverse();
            a2->reverse();
          }
          // Now bounce the asteroids off of each other
          a1->collide(*a2);
          continue;
        }

        // Now check to see if we have a bullet hitting a bullet
        shared_ptr<Bullet> b1 = dynamic_pointer_cast<Bullet>(*it);
        shared_ptr<Bullet> b2 = dynamic_pointer_cast<Bullet>(*it2);

        if (b1 && b2) {
          it = sprites.erase(it);
          sprites.erase(it2);
          goto next_iteration;
        }

        // Now check to see if we have a bullet hitting an asteroid
        Asteroid *a = NULL;
        shared_ptr<Bullet> b;
        if (a1)
          a = a1;
        else if (a2)
          a = a2;
        if (b1)
          b = b1;
        else if (b2)
          b = b2;
        if (a && b) {
          // Spawn baby asteroids if the asteroid is
          // big enough and hasn't been disabled
          Asteroid *children[4];
          if (a->createChildren(children)) {
            // Children created successfully
            for (int i = 0; i < 4; i++) {
              shared_ptr<Sprite> child(children[i]);
              add_list.push_back(child);
            }
            audio->playSample(r_explosion_sample);
          } else {
            audio->playSample(r_smallexplosion_sample);
          }
          Point position = a->getLocation();
          add_list.push_back(
              shared_ptr<Sprite>(new Explosion(position.x, position.y)));
          score += static_cast<int>(a->getSize());

          a->disable();
          sprites.erase(it2);
          it = sprites.erase(it);
          goto next_iteration;
        }

        // Did something hit the player then?
        if ((*it == player || *it2 == player) && player->isAlive()) {
          // Just kill the player.
          audio->playSample(r_explosion_sample);
          player->kill();
        }
      }
      it2++;
    }
    {
      shared_ptr<Explosion> e = dynamic_pointer_cast<Explosion>(*it);
      if (e && e->isFinished()) {
        it = sprites.erase(it);
        goto next_iteration;
      }
    }
    it++;
  next_iteration:;
  }

  // Prune new objects added on top of existing ones.
  list_it = add_list.begin();
  while (list_it != add_list.end()) {
    bool erase = false;
    for (it = sprites.begin(); it != sprites.end(); it++) {
      if ((*list_it)->detectCollision(**it)) {
        erase = true;
        break;
      }
    }
    it = list_it;
    for (it++; it != add_list.end(); it++) {
      if ((*list_it)->detectCollision(**it)) {
        erase = true;
        break;
      }
    }
    if (erase)
      list_it = add_list.erase(list_it);
    else
      list_it++;
  }
  // Now add all the objects we created in this update step
  for (list_it = add_list.begin(); list_it != add_list.end(); list_it++) {
    sprites.push_back(*list_it);
  }
}

void Game::draw() {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0f, 1.0f, 1.0f);

  glEnable(GL_SCISSOR_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glScissor(left, bottom, width, height);
  for_each(sprites.begin(), sprites.end(), bind(&Sprite::draw, _1));
  glDisable(GL_BLEND);
  glDisable(GL_SCISSOR_TEST);

  glPushAttrib(GL_CURRENT_BIT);
  // Draw border of field area
  glColor3f(1.0f, 1.0f, 1.0f);
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex2i(-100, -100);
  glVertex2i(100, -100);
  glVertex2i(100, -100);
  glVertex2i(100, 100);
  glVertex2i(100, 100);
  glVertex2i(-100, 100);
  glVertex2i(-100, 100);
  glVertex2i(-100, -100);
  glEnd();
  glPopAttrib();

  if (lives == 0 && !player->isAlive())
    drawGameOverOverlay();
  else
    drawGameOverlay();

  SDL_GL_SwapBuffers();
}

bool Game::poll() {
  bool quit = false;
  static AudioHandle thrustSample = -1;

  // Poll Events
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_UP:
          case SDLK_w:
            player->setAccelerate();
            if (player->isAlive())
              thrustSample = audio->playSample(r_thrust_sample, -1);
            break;
          case SDLK_LEFT:
          case SDLK_a:
            player->setLeft();
            break;
          case SDLK_RIGHT:
          case SDLK_d:
            player->setRight();
            break;
          case SDLK_ESCAPE:
            quit = true;
            break;
          default:
            break;
        }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
          case SDLK_UP:
          case SDLK_w:
            player->clearAccelerate();
            if (thrustSample != -1) audio->stopSample(thrustSample);
            thrustSample = -1;
            break;
          case SDLK_LEFT:
          case SDLK_a:
            player->clearLeft();
            break;
          case SDLK_RIGHT:
          case SDLK_d:
            player->clearRight();
            break;
          case SDLK_SPACE:
            if (player->isAlive()) {
              sprites.push_back(shared_ptr<Sprite>(player->fire()));
              audio->playSample(r_fire_sample);
            } else if (lives > 0 && player->isReady()) {
              lives--;
              player->revive();
            } else if (player->isReady()) {
              startGame();
              player->revive();
            }
            break;
          case SDLK_F11:
            toggleFullScreen();
            break;
          default:
            break;
        }
        break;
      case SDL_VIDEORESIZE:
        screen =
            SDL_SetVideoMode(event.resize.w, event.resize.h, 0, screen->flags);
        reshape(event.resize.w, event.resize.h);
        break;
      case SDL_QUIT:
        quit = true;
        break;
      default:
        break;
    }
  }
  return quit;
}

void Game::startGame() {
  const Point origin = {0.0f, 0.0f};
  lives = 2;
  score = 0;
  level = 1;
  player->setLocation(origin);
  sprites.clear();
  sprites.push_back(player);
}

void Game::drawGameOverOverlay() {
  DrawText(-10.0f, 8.0f, "Game Over");
  DrawText(-10.0f, -10.0f, "Press Space");
}

void Game::drawGameOverlay() {
  string sscore;
  stringstream ss;

  ss << "Score : " << score << endl;
  getline(ss, sscore);

  DrawText(-100.0f, -105.0f, sscore.c_str());

  for (int i = 0; i < lives; i++) {
    player->drawIcon(70.0f + 8.0f * i, -105.0f);
  }
}
