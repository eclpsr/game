#include "SFML/Graphics.hpp"
using namespace sf;
int main()
{
    RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

    Texture herotexture; // создаем объект Texture (текстура).
    herotexture.loadFromFile("src/images/hero.png"); //загружаем в него файл

    Sprite herosprite; //создаем объект Texture (текстура)
    herosprite.setTexture(herotexture); //передаем в него объект Image (изображения)
    herosprite.setTextureRect(IntRect(0,192,96,96)); //получили нужный нам прямоугольник
    herosprite.setPosition(250, 250); //выводим спрайт в позицию х у

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
        	herosprite.move(-0.1, 0);
        	herosprite.setTextureRect(IntRect(0, 96, 96, 96));
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
        	herosprite.move(0.1, 0);
        	herosprite.setTextureRect(IntRect(0, 192, 96, 96));
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
        	herosprite.move(0, -0.1);
        	herosprite.setTextureRect(IntRect(0, 288, 96, 96));
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
        	herosprite.move(0, 0.1);
        	herosprite.setTextureRect(IntRect(0, 0, 96, 96));
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
        	herosprite.setColor(Color::Red);
        }

        window.clear();
        window.draw(herosprite);
        window.display();
    }

    return 0;
}
