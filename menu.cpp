#include "menu.h"

Menu::Menu(float width, float height)
{
    setPosition(width, height);
}

void Menu::setPosition(float width, float height)
{
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Nie udalo sie zaladowac czcionki!" << std::endl;
    }

    title.setFont(font);
    title.setString("ARKANOID");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Cyan);
    title.setStyle(sf::Text::Bold);
    title.setPosition(width / 2 - title.getGlobalBounds().width / 2, 50);

    menu[0].setFont(font);
    menu[0].setFillColor(sf::Color::Cyan);
    menu[0].setStyle(sf::Text::Bold);
    menu[0].setString("Nowa gra");
    menu[0].setPosition(width / 2 - menu[0].getGlobalBounds().width / 2,
        height / (MAX_LICZBA_POZIOMOW + 2) * 2);

    menu[1].setFont(font);
    menu[1].setFillColor(sf::Color::White);
    menu[1].setStyle(sf::Text::Regular);
    menu[1].setString("Wczytaj gre");
    menu[1].setPosition(width / 2 - menu[1].getGlobalBounds().width / 2,
        height / (MAX_LICZBA_POZIOMOW + 2) * 3);

    menu[2].setFont(font);
    menu[2].setFillColor(sf::Color::White);
    menu[2].setStyle(sf::Text::Regular);
    menu[2].setString("Wyniki");
    menu[2].setPosition(width / 2 - menu[2].getGlobalBounds().width / 2,
        height / (MAX_LICZBA_POZIOMOW + 2) * 4);

    menu[3].setFont(font);
    menu[3].setFillColor(sf::Color::White);
    menu[3].setStyle(sf::Text::Regular);
    menu[3].setString("Wyjscie");
    menu[3].setPosition(width / 2 - menu[3].getGlobalBounds().width / 2,
        height / (MAX_LICZBA_POZIOMOW + 2) * 5);
}

void Menu::draw(sf::RenderWindow& window)
{
    window.draw(title);
    for (int i = 0; i < MAX_LICZBA_POZIOMOW; i++)
    {
        window.draw(menu[i]);
    }
}

void Menu::przesunG()
{
    if (selectedItem >= 0 && selectedItem < MAX_LICZBA_POZIOMOW)
    {
        menu[selectedItem].setFillColor(sf::Color::White);
        menu[selectedItem].setStyle(sf::Text::Regular);
        selectedItem--;

        if (selectedItem < 0)
            selectedItem = MAX_LICZBA_POZIOMOW - 1;

        menu[selectedItem].setFillColor(sf::Color::Cyan);
        menu[selectedItem].setStyle(sf::Text::Bold);
    }
}

void Menu::przesunD()
{
    if (selectedItem >= 0 && selectedItem < MAX_LICZBA_POZIOMOW)
    {
        menu[selectedItem].setFillColor(sf::Color::White);
        menu[selectedItem].setStyle(sf::Text::Regular);
        selectedItem++;

        if (selectedItem >= MAX_LICZBA_POZIOMOW)
            selectedItem = 0;

        menu[selectedItem].setFillColor(sf::Color::Cyan);
        menu[selectedItem].setStyle(sf::Text::Bold);
    }
}

void Menu::resetSelection()
{
    for (int i = 0; i < MAX_LICZBA_POZIOMOW; i++)
    {
        menu[i].setFillColor(sf::Color::White);
        menu[i].setStyle(sf::Text::Regular);
    }
    selectedItem = 0;
    menu[0].setFillColor(sf::Color::Cyan);
    menu[0].setStyle(sf::Text::Bold);
}

void Menu::setTitle(const std::string& text)
{
    title.setString(text);
}

MenuZapisow::MenuZapisow(float width, float height, const std::vector<std::string>& listaZapisow)
{
    setPosition(width, height);
    setListaZapisow(listaZapisow);
}

void MenuZapisow::setPosition(float width, float height)
{
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Nie udalo sie zaladowac czcionki!" << std::endl;
    }

    title.setFont(font);
    title.setString("WCZYTAJ GRE");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::Yellow);
    title.setStyle(sf::Text::Bold);
    title.setPosition(width / 2 - title.getGlobalBounds().width / 2, 30);

    info.setFont(font);
    info.setString("Strzalki: Wybor | Enter: Wczytaj | ESC: Powrot");
    info.setCharacterSize(20);
    info.setFillColor(sf::Color::White);
    info.setPosition(width / 2 - info.getGlobalBounds().width / 2, height - 50);
}

void MenuZapisow::setListaZapisow(const std::vector<std::string>& listaZapisow)
{
    zapisy.clear();

    float width = 800;
    float height = 600;
    float startY = 100;
    float spacing = 35;

    int ilosc = static_cast<int>(std::min(listaZapisow.size(),
        static_cast<size_t>(MAX_ZAPISOW_W_LISCIE)));

    for (int i = 0; i < ilosc; i++)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(i + 1) + ". " + listaZapisow[i]);
        text.setCharacterSize(28);
        text.setPosition(width / 2 - text.getGlobalBounds().width / 2,
            startY + i * spacing);

        if (i == 0)
        {
            text.setFillColor(sf::Color::Cyan);
            text.setStyle(sf::Text::Bold);
        }
        else
        {
            text.setFillColor(sf::Color::White);
            text.setStyle(sf::Text::Regular);
        }

        zapisy.push_back(text);
    }

    if (zapisy.empty())
    {
        sf::Text text;
        text.setFont(font);
        text.setString("Brak zapisanych gier");
        text.setCharacterSize(28);
        text.setFillColor(sf::Color::Red);
        text.setPosition(width / 2 - text.getGlobalBounds().width / 2, startY);
        zapisy.push_back(text);
    }
}

void MenuZapisow::draw(sf::RenderWindow& window)
{
    window.draw(title);
    window.draw(info);

    for (auto& text : zapisy)
    {
        window.draw(text);
    }
}

void MenuZapisow::przesunG()
{
    if (!zapisy.empty() && selectedItem >= 0 &&
        selectedItem < static_cast<int>(zapisy.size()))
    {
        zapisy[selectedItem].setFillColor(sf::Color::White);
        zapisy[selectedItem].setStyle(sf::Text::Regular);
        selectedItem--;

        if (selectedItem < 0)
            selectedItem = static_cast<int>(zapisy.size()) - 1;

        zapisy[selectedItem].setFillColor(sf::Color::Cyan);
        zapisy[selectedItem].setStyle(sf::Text::Bold);
    }
}

void MenuZapisow::przesunD()
{
    if (!zapisy.empty() && selectedItem >= 0 &&
        selectedItem < static_cast<int>(zapisy.size()))
    {
        zapisy[selectedItem].setFillColor(sf::Color::White);
        zapisy[selectedItem].setStyle(sf::Text::Regular);
        selectedItem++;

        if (selectedItem >= static_cast<int>(zapisy.size()))
            selectedItem = 0;

        zapisy[selectedItem].setFillColor(sf::Color::Cyan);
        zapisy[selectedItem].setStyle(sf::Text::Bold);
    }
}

std::string MenuZapisow::getWybranyZapis()
{
    if (zapisy.empty() || selectedItem < 0 ||
        selectedItem >= static_cast<int>(zapisy.size()))
        return "";

    std::string text = zapisy[selectedItem].getString();

    size_t dotPos = text.find(". ");
    if (dotPos != std::string::npos)
    {
        return text.substr(dotPos + 2);
    }

    return text;
}

void MenuZapisow::resetSelection()
{
    selectedItem = 0;
    if (!zapisy.empty())
    {
        for (auto& text : zapisy)
        {
            text.setFillColor(sf::Color::White);
            text.setStyle(sf::Text::Regular);
        }
        zapisy[0].setFillColor(sf::Color::Cyan);
        zapisy[0].setStyle(sf::Text::Bold);
    }
}