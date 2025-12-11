#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "menu.h"
#include "game.h"

void zapiszWynik(int wynik) {
    std::vector<int> wyniki;

    std::ifstream plikOdczyt("wyniki.txt");
    if (plikOdczyt.is_open()) {
        int w;
        while (plikOdczyt >> w) {
            wyniki.push_back(w);
        }
        plikOdczyt.close();
    }

    wyniki.push_back(wynik);

    std::sort(wyniki.rbegin(), wyniki.rend());

    if (wyniki.size() > 5) {
        wyniki.resize(5);
    }

    std::ofstream plikZapis("wyniki.txt");
    if (plikZapis.is_open()) {
        for (int w : wyniki) {
            plikZapis << w << std::endl;
        }
        plikZapis.close();
        std::cout << "Wynik zapisany do tabeli!" << std::endl;
    }
}

std::vector<int> wczytajWyniki() {
    std::vector<int> wyniki;

    std::ifstream plik("wyniki.txt");
    if (plik.is_open()) {
        int w;
        while (plik >> w) {
            wyniki.push_back(w);
        }
        plik.close();
    }

    std::sort(wyniki.rbegin(), wyniki.rend());

    if (wyniki.size() > 5) {
        wyniki.resize(5);
    }

    while (wyniki.size() < 5) {
        wyniki.push_back(0);
    }

    return wyniki;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "ARKANOID PRO");
    window.setFramerateLimit(60);

    Menu menu(static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y));
    Game game(&window);

    std::vector<std::string> listaZapisow = Game::znajdzZapisy();

    if (listaZapisow.size() > 10) {
        listaZapisow.resize(10);
    }

    MenuZapisow menuZapisow(static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y),
        listaZapisow);

    enum class GameState { MENU, WYBIERZ_ZAPIS, PLAYING, SCORES, GAME_OVER };
    GameState currentState = GameState::MENU;

    bool gameActive = false;
    int finalScore = 0;
    std::vector<int> najlepszeWyniki = wczytajWyniki();

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                switch (currentState)
                {
                case GameState::MENU:
                    if (event.key.code == sf::Keyboard::Up)
                        menu.przesunG();
                    else if (event.key.code == sf::Keyboard::Down)
                        menu.przesunD();
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        switch (menu.getSelectedItem())
                        {
                        case 0:
                            std::cout << "\n=== NOWA GRA ===" << std::endl;
                            game.initialize();
                            gameActive = true;
                            currentState = GameState::PLAYING;
                            break;
                        case 1:
                            listaZapisow = Game::znajdzZapisy();
                            if (listaZapisow.size() > 10) {
                                listaZapisow.resize(10);
                            }
                            menuZapisow.setListaZapisow(listaZapisow);
                            menuZapisow.resetSelection();
                            currentState = GameState::WYBIERZ_ZAPIS;
                            break;
                        case 2:
                            std::cout << "Pokaz wyniki..." << std::endl;
                            najlepszeWyniki = wczytajWyniki();
                            currentState = GameState::SCORES;
                            break;
                        case 3:
                            window.close();
                            break;
                        }
                    }
                    break;

                case GameState::WYBIERZ_ZAPIS:
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        currentState = GameState::MENU;
                        menu.resetSelection();
                    }
                    else if (event.key.code == sf::Keyboard::Up)
                    {
                        menuZapisow.przesunG();
                    }
                    else if (event.key.code == sf::Keyboard::Down)
                    {
                        menuZapisow.przesunD();
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        std::string wybranyZapis = menuZapisow.getWybranyZapis();
                        if (!wybranyZapis.empty() && wybranyZapis != "Brak zapisanych gier")
                        {
                            if (game.wczytajGre(wybranyZapis))
                            {
                                gameActive = true;
                                currentState = GameState::PLAYING;
                            }
                        }
                    }
                    break;

                case GameState::PLAYING:
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        currentState = GameState::MENU;
                        gameActive = false;
                        menu.resetSelection();
                    }
                    break;

                case GameState::SCORES:
                    if (event.key.code == sf::Keyboard::Escape ||
                        event.key.code == sf::Keyboard::Enter)
                    {
                        currentState = GameState::MENU;
                        menu.resetSelection();
                    }
                    break;

                case GameState::GAME_OVER:
                    if (event.key.code == sf::Keyboard::Escape ||
                        event.key.code == sf::Keyboard::Enter)
                    {
                        currentState = GameState::MENU;
                        menu.resetSelection();
                    }
                    break;
                }
            }
        }

        if (currentState == GameState::PLAYING && gameActive)
        {
            game.processEvents();

            if (game.isRunning())
            {
                game.update();
                game.render();
            }
            else
            {
                finalScore = game.getScore();
                zapiszWynik(finalScore);
                najlepszeWyniki = wczytajWyniki();
                currentState = GameState::GAME_OVER;
                gameActive = false;
                menu.resetSelection();
            }
        }

        window.clear(sf::Color::Black);

        switch (currentState)
        {
        case GameState::MENU:
            menu.draw(window);
            break;

        case GameState::WYBIERZ_ZAPIS:
            menuZapisow.draw(window);
            break;

        case GameState::PLAYING:
            if (gameActive)
                game.render();
            break;

        case GameState::SCORES:
        {
            sf::RectangleShape background(sf::Vector2f(800, 600));
            background.setFillColor(sf::Color(0, 0, 50));
            window.draw(background);

            sf::Font font;
            if (font.loadFromFile("arial.ttf"))
            {
                sf::Text title;
                title.setFont(font);
                title.setString("TOP 5 WYNIKOW");
                title.setCharacterSize(50);
                title.setFillColor(sf::Color::Yellow);
                title.setStyle(sf::Text::Bold);
                title.setPosition(400 - title.getLocalBounds().width / 2, 50);
                window.draw(title);

                std::string wynikTekst;
                for (size_t i = 0; i < najlepszeWyniki.size(); ++i) {
                    if (najlepszeWyniki[i] > 0) {
                        wynikTekst += std::to_string(i + 1) + ". " + std::to_string(najlepszeWyniki[i]) + " pkt\n";
                    }
                    else {
                        wynikTekst += std::to_string(i + 1) + ". ---\n";
                    }
                }

                sf::Text scores;
                scores.setFont(font);
                scores.setString(wynikTekst);
                scores.setCharacterSize(40);
                scores.setFillColor(sf::Color::White);
                scores.setPosition(400 - scores.getLocalBounds().width / 2, 150);
                scores.setLineSpacing(1.2f);
                window.draw(scores);

                sf::Text instruction;
                instruction.setFont(font);
                instruction.setString("ESC lub ENTER - powrot");
                instruction.setCharacterSize(22);
                instruction.setFillColor(sf::Color(200, 200, 200));
                instruction.setPosition(400 - instruction.getLocalBounds().width / 2, 500);
                window.draw(instruction);
            }
            break;
        }

        case GameState::GAME_OVER:
        {
            sf::RectangleShape background(sf::Vector2f(800, 600));
            background.setFillColor(sf::Color(50, 0, 0));
            window.draw(background);

            sf::Font font;
            if (font.loadFromFile("arial.ttf"))
            {
                sf::Text gameOver;
                gameOver.setFont(font);
                gameOver.setString("KONIEC GRY");
                gameOver.setCharacterSize(60);
                gameOver.setFillColor(sf::Color::Red);
                gameOver.setStyle(sf::Text::Bold);
                gameOver.setPosition(400 - gameOver.getLocalBounds().width / 2, 100);
                window.draw(gameOver);

                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setString("Twoj wynik: " + std::to_string(finalScore) + " pkt");
                scoreText.setCharacterSize(40);
                scoreText.setFillColor(sf::Color::Yellow);
                scoreText.setPosition(400 - scoreText.getLocalBounds().width / 2, 200);
                window.draw(scoreText);

                bool jestWTop5 = false;
                for (int wynik : najlepszeWyniki) {
                    if (finalScore == wynik) {
                        jestWTop5 = true;
                        break;
                    }
                }

                if (jestWTop5) {
                    sf::Text gratulacje;
                    gratulacje.setFont(font);
                    gratulacje.setString("GRATULACJE! Nowy rekord!");
                    gratulacje.setCharacterSize(30);
                    gratulacje.setFillColor(sf::Color::Green);
                    gratulacje.setPosition(400 - gratulacje.getLocalBounds().width / 2, 260);
                    window.draw(gratulacje);
                }

                sf::Text instruction;
                instruction.setFont(font);
                instruction.setString("ESC lub ENTER - powrot do menu");
                instruction.setCharacterSize(22);
                instruction.setFillColor(sf::Color::White);
                instruction.setPosition(400 - instruction.getLocalBounds().width / 2, 320);
                window.draw(instruction);
            }
            break;
        }
        }

        window.display();
    }

    return 0;
}