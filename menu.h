#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#define MAX_LICZBA_POZIOMOW 4
#define MAX_ZAPISOW_W_LISCIE 10

class Menu
{
private:
    sf::Font font;
    sf::Text menu[MAX_LICZBA_POZIOMOW];
    int selectedItem = 0;
    sf::Text title;

public:
    Menu(float width, float height);
    ~Menu() {};

    void przesunG();
    void przesunD();

    int getSelectedItem() { return selectedItem; }
    void draw(sf::RenderWindow& window);
    void resetSelection();
    void setPosition(float width, float height);

    void setTitle(const std::string& text);
};

class MenuZapisow
{
private:
    sf::Font font;
    std::vector<sf::Text> zapisy;
    int selectedItem = 0;
    sf::Text title;
    sf::Text info;

public:
    MenuZapisow(float width, float height, const std::vector<std::string>& listaZapisow);

    void przesunG();
    void przesunD();

    int getSelectedItem() { return selectedItem; }
    std::string getWybranyZapis();
    void draw(sf::RenderWindow& window);
    void resetSelection();
    void setPosition(float width, float height);

    void setListaZapisow(const std::vector<std::string>& listaZapisow);
};

#endif