#include "game.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

std::string getCurrentTimestamp() {
    std::time_t t = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &t);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y%m%d_%H%M%S");
    return oss.str();
}

Game::Game(sf::RenderWindow* win) : window(win), gameRunning(false),
blocksRemaining(0), score(0),
fontLoaded(false), showSaveMessage(false), saveMessageTimer(0),
zPressedLastFrame(false)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void Game::initialize()
{
    gameRunning = true;
    score = 0;
    showSaveMessage = false;
    saveMessageTimer = 0;
    zPressedLastFrame = false;

    paletka.SetProp(SCREENWIDTH / 2, SCREENHEIGHT - 50, 120.f, 20.f, 10.f);
    paletka.setColor(sf::Color(100, 200, 255));

    resetBall();

    if (!fontLoaded)
    {
        fontLoaded = font.loadFromFile("arial.ttf");
    }

    createBlocks();

    std::cout << "=== NOWA GRA ===" << std::endl;
}

void Game::resetBall()
{
    float randomVx = static_cast<float>(std::rand() % 3 + 3) * (std::rand() % 2 == 0 ? 1.f : -1.f);
    float randomVy = -static_cast<float>(std::rand() % 3 + 3);
    pilka.SetProp(SCREENWIDTH / 2, SCREENHEIGHT - 80, randomVx, randomVy, 10.f);
    pilka.setColor(sf::Color(255, 200, 100));
}

void Game::createBlocks()
{
    float blockWidth = (SCREENWIDTH - (cols + 1) * BOXSPACINGHORIZONTAL) / cols;
    float blockHeight = 30;

    blocksRemaining = 0;
    bloki.clear();

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            float bx = blockWidth / 2 + BOXSPACINGHORIZONTAL + c * (blockWidth + BOXSPACINGHORIZONTAL);
            float by = 100 + blockHeight / 2 + r * (blockHeight + BOXSPACINGVERTICAL);

            Blok blok;
            int typBloku = std::rand() % 100;
            sf::Color kolor;
            int punkty;
            int zyciaBloku;

            if (typBloku < 40)
            {
                kolor = sf::Color::Yellow;
                punkty = 100;
                zyciaBloku = 1;
            }
            else if (typBloku < 70)
            {
                kolor = sf::Color::Blue;
                punkty = 200;
                zyciaBloku = 2;
            }
            else if (typBloku < 85)
            {
                kolor = sf::Color::Red;
                punkty = 300;
                zyciaBloku = 3;
            }
            else
            {
                kolor = sf::Color::Green;
                punkty = 50;
                zyciaBloku = 1;
            }

            blok.setProp(bx, by, blockWidth, blockHeight, kolor, punkty, zyciaBloku);
            bloki.push_back(blok);
            blocksRemaining++;
        }
    }

    std::cout << "Stworzono " << blocksRemaining << " blokow" << std::endl;
}

std::vector<std::string> Game::znajdzZapisy()
{
    std::vector<std::string> zapisy;

    try {
        for (const auto& entry : fs::directory_iterator("."))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();
                if (filename.find("zapis_gry_") == 0 && filename.find(".txt") != std::string::npos)
                {
                    zapisy.push_back(filename);
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << "Blad przeszukiwania folderu: " << e.what() << std::endl;
    }

    std::sort(zapisy.rbegin(), zapisy.rend());

    if (zapisy.size() > 10) {
        zapisy.resize(10);
    }

    return zapisy;
}

bool Game::zapiszGre(const std::string& nazwaPliku)
{
    std::string pelnaNazwa = "zapis_gry_" + nazwaPliku + ".txt";
    std::ofstream plik(pelnaNazwa);

    if (!plik.is_open())
    {
        std::cout << "Nie mozna utworzyc pliku zapisu!" << std::endl;
        return false;
    }


    plik << score << std::endl;
    plik << blocksRemaining << std::endl;

    plik << paletka.getX() << " " << paletka.getY() << std::endl;

    plik << pilka.getX() << " " << pilka.getY() << " "
        << pilka.getVx() << " " << pilka.getVy() << std::endl;

    int zyweBloki = 0;
    for (const auto& blok : bloki) {
        if (!blok.IsDead()) zyweBloki++;
    }
    plik << zyweBloki << std::endl;

    for (const auto& blok : bloki) {
        if (!blok.IsDead()) {
            sf::Color kolor = blok.getColor();
            plik << blok.getX() << " " << blok.getY() << " "
                << blok.getWidth() << " " << blok.getHeight() << " "
                << blok.getPunkty() << " " << blok.getZycia() << " "
                << static_cast<int>(kolor.r) << " "
                << static_cast<int>(kolor.g) << " "
                << static_cast<int>(kolor.b) << std::endl;
        }
    }

    plik.close();

    showSaveMessage = true;
    saveMessageTimer = 90;

    std::cout << "Gra zapisana jako: " << pelnaNazwa << std::endl;
    return true;
}

bool Game::wczytajGre(const std::string& nazwaPliku)
{
    std::ifstream plik(nazwaPliku);
    if (!plik.is_open())
    {
        std::cout << "Nie mozna otworzyc pliku: " << nazwaPliku << std::endl;
        return false;
    }

    plik >> score;
    plik >> blocksRemaining;

    float px, py;
    plik >> px >> py;
    paletka.SetProp(px, py, 120.f, 20.f, 10.f);
    paletka.setColor(sf::Color(100, 200, 255));

    float bx, by, vx, vy;
    plik >> bx >> by >> vx >> vy;
    pilka.SetProp(bx, by, vx, vy, 10.f);
    pilka.setColor(sf::Color(255, 200, 100));

    int liczbaBlokow;
    plik >> liczbaBlokow;

    bloki.clear();

    for (int i = 0; i < liczbaBlokow; i++)
    {
        float x, y, width, height;
        int punkty, zycia;
        int r, g, b;

        plik >> x >> y >> width >> height >> punkty >> zycia >> r >> g >> b;

        Blok nowyBlok;
        sf::Color kolor(r, g, b);
        nowyBlok.setProp(x, y, width, height, kolor, punkty, zycia);
        bloki.push_back(nowyBlok);
    }

    plik.close();
    gameRunning = true;

    std::cout << "Gra wczytana: " << nazwaPliku << std::endl;
    std::cout << "Wynik: " << score << ", Bloki: " << blocksRemaining << std::endl;

    return true;
}

void Game::processEvents()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
            gameRunning = false;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                gameRunning = false;
            }

            if (event.key.code == sf::Keyboard::Space)
            {
                resetBall();
                std::cout << "Nowa pilka!" << std::endl;
            }
        }
    }
}

void Game::update()
{

    if (showSaveMessage)
    {
        saveMessageTimer--;
        if (saveMessageTimer <= 0)
        {
            showSaveMessage = false;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        if (!zPressedLastFrame)
        {
            std::cout << "\n=== ZAPIS GRY ===" << std::endl;
            std::string timestamp = getCurrentTimestamp();
            if (zapiszGre(timestamp))
            {
                std::cout << ">>> ZAPIS UDANY <<<" << std::endl;
            }
            else
            {
                std::cout << ">>> ZAPIS NIEUDANY <<<" << std::endl;
            }
        }
        zPressedLastFrame = true;
    }
    else
    {
        zPressedLastFrame = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        paletka.moveLeft();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        paletka.moveRight();
    paletka.clampToBounds(SCREENWIDTH);

    pilka.move();
    pilka.collideWalls(SCREENWIDTH, SCREENHEIGHT);

    if (pilka.collidePaddle(paletka))
    {
        score += 5;
    }

    if (pilka.getY() - pilka.getRadius() > SCREENHEIGHT)
    {
        std::cout << "PILKA SPADLA! KONIEC GRY! Wynik: " << score << " punktow" << std::endl;
        gameRunning = false;
    }

    for (auto& blok : bloki)
    {
        if (blok.CheckCollision(pilka))
        {
            score += blok.getPunkty();
            if (blok.IsDead())
            {
                blocksRemaining--;
            }
        }
    }

    if (blocksRemaining == 0)
    {
        std::cout << "WYGRANA! Wynik: " << score << " punktow" << std::endl;
        gameRunning = false;
    }
}

void Game::drawHUD()
{
    if (fontLoaded)
    {
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Wynik: " + std::to_string(score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::Yellow);
        scoreText.setPosition(10, 10);
        window->draw(scoreText);

        sf::Text blocksText;
        blocksText.setFont(font);
        blocksText.setString("Bloki: " + std::to_string(blocksRemaining));
        blocksText.setCharacterSize(24);
        blocksText.setFillColor(sf::Color::Cyan);
        blocksText.setPosition(10, 40);
        window->draw(blocksText);

        sf::Text helpText;
        helpText.setFont(font);
        helpText.setString("Strzalki: Sterowanie | ESC: Menu | Z: Zapisz ");
        helpText.setCharacterSize(18);
        helpText.setFillColor(sf::Color(200, 200, 200));
        helpText.setPosition(SCREENWIDTH - helpText.getLocalBounds().width - 10, 10);
        window->draw(helpText);

        if (showSaveMessage)
        {
            sf::Text saveText;
            saveText.setFont(font);
            saveText.setString("Gra zapisana!");
            saveText.setCharacterSize(30);
            saveText.setFillColor(sf::Color::Green);
            saveText.setStyle(sf::Text::Bold);
            saveText.setPosition(SCREENWIDTH / 2 - saveText.getLocalBounds().width / 2, SCREENHEIGHT - 100);
            window->draw(saveText);
        }
    }
}

void Game::render()
{
    window->clear(sf::Color(20, 20, 40));

    paletka.draw(*window);
    pilka.draw(*window);

    for (auto& blok : bloki)
        blok.draw(*window);

    drawHUD();
}