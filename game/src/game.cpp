#include "SFML/Graphics.hpp"
using namespace sf;
int main()
{
    RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

    Texture herotexture; // создаем объект Texture (текстура).
    herotexture.loadFromFile("src/images/hero.png"); //загружаем в него файл

    Sprite herosprite; //создаем объект Texture (текстура)
    herosprite.setTexture(herotexture); //передаем в него объект Image (изображения)
    herosprite.setTextureRect(IntRect(0,192,96,96));
    herosprite.setPosition(50, 25); //задаем начальные координаты появления спрайта

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
