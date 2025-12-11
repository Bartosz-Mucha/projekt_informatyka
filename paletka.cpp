#include "paletka.h"
#include <SFML/Graphics.hpp>

Paletka::Paletka() : x(0), y(0), szerokosc(0), wysokosc(0), predkosc(0)
{
}

void Paletka::SetProp(float X, float Y, float W, float H, float VX)
{
    x = X; y = Y;
    szerokosc = W; wysokosc = H;
    predkosc = VX;

    shape.setSize(sf::Vector2f(szerokosc, wysokosc));
    shape.setOrigin(szerokosc / 2, wysokosc / 2);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Cyan);
}

void Paletka::moveLeft()
{
    x -= predkosc;
    shape.setPosition(x, y);
}

void Paletka::moveRight()
{
    x += predkosc;
    shape.setPosition(x, y);
}

void Paletka::clampToBounds(float width)
{
    if (x - szerokosc / 2 < 0) x = szerokosc / 2;
    if (x + szerokosc / 2 > width) x = width - szerokosc / 2;
    shape.setPosition(x, y);
}

void Paletka::draw(sf::RenderTarget& target)
{
    target.draw(shape);
}

float Paletka::getX() const { return x; }
float Paletka::getY() const { return y; }
float Paletka::getSzerokosc() const { return szerokosc; }
float Paletka::getWysokosc() const { return wysokosc; }
sf::Color Paletka::getColor() const { return shape.getFillColor(); }

void Paletka::setPosition(float newX, float newY)
{
    x = newX;
    y = newY;
    shape.setPosition(x, y);
}

void Paletka::setSize(float width, float height)
{
    szerokosc = width;
    wysokosc = height;
    shape.setSize(sf::Vector2f(szerokosc, wysokosc));
    shape.setOrigin(szerokosc / 2, wysokosc / 2);
}

void Paletka::setColor(const sf::Color& color)
{
    shape.setFillColor(color);
}