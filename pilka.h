#ifndef PILKA_H
#define PILKA_H

#include <SFML/Graphics.hpp>
#include "paletka.h"

class Pilka
{
private:
    float x, y;
    float vx, vy;
    float radius;
    sf::CircleShape shape;

public:
    Pilka();
    void SetProp(float X, float Y, float VX, float VY, float RAD);

    void move();
    void bounceX();
    void bounceY();
    void collideWalls(float width, float height);
    bool collidePaddle(const Paletka& p);

    void draw(sf::RenderTarget& target);

    float getX() const;
    float getY() const;
    float getVx() const;
    float getVy() const;
    float getRadius() const;

    void setPosition(float newX, float newY);
    void setVelocity(float newVx, float newVy);
    void setColor(const sf::Color& color);
};

#endif