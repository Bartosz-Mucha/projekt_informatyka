#ifndef PALETKA_H
#define PALETKA_H

#include <SFML/Graphics.hpp>

class Paletka
{
private:
    float x, y;
    float szerokosc, wysokosc;
    float predkosc;
    sf::RectangleShape shape;

public:
    Paletka();
    void SetProp(float X, float Y, float W, float H, float VX);

    void moveLeft();
    void moveRight();
    void clampToBounds(float width);

    void draw(sf::RenderTarget& target);

    float getX() const;
    float getY() const;
    float getSzerokosc() const;
    float getWysokosc() const;
    sf::Color getColor() const;

    void setPosition(float newX, float newY);
    void setSize(float width, float height);
    void setColor(const sf::Color& color);
};

#endif