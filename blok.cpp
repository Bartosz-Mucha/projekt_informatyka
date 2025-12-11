#include "blok.h"
#include "pilka.h"
#include <iostream>

Blok::Blok() : x(0), y(0), width(0), height(0), DeadFlag(false),
punkty(0), zycia(1), originalColor(sf::Color::White)
{
}

void Blok::setProp(float X, float Y, float W, float H, sf::Color kolor, int pkt, int zy)
{
    x = X;
    y = Y;
    width = W;
    height = H;
    DeadFlag = false;
    punkty = pkt;
    zycia = zy;
    originalColor = kolor;

    shape.setSize(sf::Vector2f(width, height));
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(x, y);
    shape.setFillColor(kolor);
    shape.setOutlineThickness(1);
    shape.setOutlineColor(sf::Color::Black);
}

void Blok::draw(sf::RenderTarget& target)
{
    if (!DeadFlag)
    {
        target.draw(shape);
    }
}

bool Blok::IsDead() const
{
    return DeadFlag;
}

bool Blok::CheckCollision(Pilka& p)
{
    if (DeadFlag)
        return false;

    float dx = std::abs(p.getX() - x);
    float dy = std::abs(p.getY() - y);

    if (dx <= (width / 2 + p.getRadius()) &&
        dy <= (height / 2 + p.getRadius()))
    {
        trafiony();

        if (dx / (width / 2 + p.getRadius()) > dy / (height / 2 + p.getRadius()))
        {
            p.bounceX();
        }
        else
        {
            p.bounceY();
        }

        return true;
    }
    return false;
}

void Blok::trafiony()
{
    zycia--;

    if (zycia <= 0)
    {
        DeadFlag = true;
        std::cout << "Blok zniszczony! +" << punkty << " punktow" << std::endl;
    }
    else
    {
        if (zycia == 1)
        {
            shape.setFillColor(sf::Color::Yellow);
            punkty = 100;
            std::cout << "Blok trafiony! Pozostalo " << zycia << " zycie. +100 pkt" << std::endl;
        }
        else if (zycia == 2)
        {
            shape.setFillColor(sf::Color::Blue);
            punkty = 200;
            std::cout << "Blok trafiony! Pozostalo " << zycia << " zycia. +200 pkt" << std::endl;
        }
    }
}

float Blok::getX() const { return x; }
float Blok::getY() const { return y; }
float Blok::getWidth() const { return width; }
float Blok::getHeight() const { return height; }
int Blok::getPunkty() const { return punkty; }
int Blok::getZycia() const { return zycia; }
sf::Color Blok::getColor() const { return shape.getFillColor(); }