#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "paletka.h"
#include "pilka.h"
#include "blok.h"

class Game
{
private:
    sf::RenderWindow* window;
    bool gameRunning;

    const float SCREENWIDTH = 800;
    const float SCREENHEIGHT = 600;
    const float BOXSPACINGHORIZONTAL = 10;
    const float BOXSPACINGVERTICAL = 5;
    const int rows = 5;
    const int cols = 8;

    Paletka paletka;
    Pilka pilka;
    std::vector<Blok> bloki;

    int blocksRemaining;
    int score;

    sf::Font font;
    bool fontLoaded;

    bool showSaveMessage;
    int saveMessageTimer;
    bool zPressedLastFrame;

public:
    Game(sf::RenderWindow* win);
    ~Game() {}

    void initialize();
    void processEvents();
    void update();
    void render();
    bool isRunning() const { return gameRunning; }
    void stop() { gameRunning = false; }

    int getScore() const { return score; }

    bool zapiszGre(const std::string& filename);
    bool wczytajGre(const std::string& filename);
    static std::vector<std::string> znajdzZapisy();

private:
    void createBlocks();
    void drawHUD();
    void resetBall();
};

#endif