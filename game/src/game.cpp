#include "SFML/Graphics.hpp"
using namespace sf;
int main()
{
    RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

    Texture herotexture; // создаём объект Texture(текстура)
    herotexture.loadFromFile("src/images/hero.png");// загружаем картинку

    Sprite herosprite; // создаём объект Sprite(спрайт)
    herosprite.setTexture(herotexture); // передаём в него объект Texture (текстуры)
    herosprite.setTextureRect(IntRect(0,192,96,96));
    herosprite.setPosition(50, 25); // задаём начальные координаты появления спрайта

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(herosprite);
        window.display();
    }

    return 0;
}
