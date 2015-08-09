// Game.h
// Copyright (c) 2011 James O'Doherty
// All Rights Reserved.

#ifndef GAME_H__
#define GAME_H__

#include <SDL.h>

#include "AudioService.h"
#include "Sprite.h"
#include "Ship.h"
#include <vector>
#include <memory>

class Game
{
    std::vector< std::shared_ptr<Sprite> > sprites;
    std::shared_ptr<Ship> player;
    int score;
    int lives;
    int level;
    int left, bottom, width, height;
    AudioService *audio;

public:
    Game();
    ~Game();

    void run();

    bool poll();
    void update();
    void draw();

private:
    // Handle screen resize event
    void reshape(int w, int h);
    // Handle fullscreen toggle event
    void toggleFullScreen();

    void startGame();
    void drawGameOverOverlay();
    void drawGameOverlay();

};

#endif // GAME_H__

