#ifndef BLOK_H
#define BLOK_H

#include <SFML/Graphics.hpp>

class Pilka;

class Blok
{
private:
    float x, y;
    float width, height;
    sf::RectangleShape shape;
    bool DeadFlag;
    int punkty;
    int zycia;
    sf::Color originalColor;

public:
    Blok();
    void setProp(float X, float Y, float W, float H, sf::Color kolor, int pkt, int zy);
    void draw(sf::RenderTarget& target);
    bool IsDead() const;
    bool CheckCollision(Pilka& p);

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    int getPunkty() const;
    int getZycia() const;
    sf::Color getColor() const;

    void trafiony();
};

#endif