// main.cpp - Game initialization and main loop
// Copyright (c) 2011 James O'Doherty
// All Rights Reserved

#include <SDL.h>

#include "Game.h"

using namespace std;

int main(int argc, char *argv[]) {
  Game game;

  game.run();
  return 0;
}
