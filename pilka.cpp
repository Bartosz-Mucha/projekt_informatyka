#include "pilka.h"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>

Pilka::Pilka() : x(0), y(0), vx(0), vy(0), radius(0)
{
}

void Pilka::SetProp(float X, float Y, float VX, float VY, float RAD)
{
    x = X; y = Y;
    vx = VX; vy = VY;
    radius = RAD;

    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Red);
}

void Pilka::move()
{
    x += vx;
    y += vy;

    x = std::max(radius, std::min(800.0f - radius, x));
    y = std::max(radius, y);

    shape.setPosition(x, y);
}

void Pilka::bounceX()
{
    vx = -vx;
}

void Pilka::bounceY()
{
    vy = -vy;
}

void Pilka::collideWalls(float width, float height)
{
    if (x - radius <= 0 || x + radius >= width)
    {
        bounceX();
        x = std::max(radius, std::min(width - radius, x));
    }

    if (y - radius <= 0)
    {
        bounceY();
        y = radius;
    }
}

bool Pilka::collidePaddle(const Paletka& p)
{
    if (y + radius >= p.getY() - p.getWysokosc() / 2)
    {
        if (x > p.getX() - p.getSzerokosc() / 2 && x < p.getX() + p.getSzerokosc() / 2)
        {
            float hitPosition = (x - p.getX()) / (p.getSzerokosc() / 2);
            hitPosition = std::max(-1.0f, std::min(1.0f, hitPosition));

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                vx = -std::abs(vx) * (1.0f + std::abs(hitPosition) * 0.3f);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                vx = std::abs(vx) * (1.0f + std::abs(hitPosition) * 0.3f);
            }
            else
            {
                vx += hitPosition * 2.0f;
                vx = std::max(-8.0f, std::min(8.0f, vx));
            }

            vy = -std::abs(vy) * 1.05f;
            move();
            return true;
        }
    }
    return false;
}

void Pilka::draw(sf::RenderTarget& target)
{
    target.draw(shape);
}

float Pilka::getX() const { return x; }
float Pilka::getY() const { return y; }
float Pilka::getVx() const { return vx; }
float Pilka::getVy() const { return vy; }
float Pilka::getRadius() const { return radius; }

void Pilka::setPosition(float newX, float newY)
{
    x = newX;
    y = newY;
    shape.setPosition(x, y);
}

void Pilka::setVelocity(float newVx, float newVy)
{
    vx = newVx;
    vy = newVy;
}

void Pilka::setColor(const sf::Color& color)
{
    shape.setFillColor(color);
}