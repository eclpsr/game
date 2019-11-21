#include "SFML/Graphics.hpp"
#include <iostream>
using namespace sf;
int main()
{
	Clock clock; // создаем переменную времени, т.о. привязка ко времени(а не загруженности/мощности процессора).

    RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

    Texture herotexture; // создаем объект Texture (текстура).
    herotexture.loadFromFile("src/images/hero.png"); //загружаем в него файл

    Sprite herosprite; //создаем объект Texture (текстура)
    herosprite.setTexture(herotexture); //передаем в него объект Image (изображения)
    herosprite.setTextureRect(IntRect(0,192,96,96)); //получили нужный нам прямоугольник
    herosprite.setPosition(250, 250); //выводим спрайт в позицию х у

    float heroteleporttimer = 0; // создаём для примера телепортации героя через 3 секунды

    while (window.isOpen())
    {
    	float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
    	clock.restart(); //перезагружает время
    	time = time/800; //скорость игры

    	heroteleporttimer += time;
    	if(heroteleporttimer > 3000) {herosprite.setPosition(0, 120); heroteleporttimer = 0; }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if ((Keyboard::isKeyPressed(Keyboard::Left)) || (Keyboard::isKeyPressed(Keyboard::A)))  {
        	herosprite.move(-0.1*time, 0);
        	herosprite.setTextureRect(IntRect(0, 96, 96, 96));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Right)) || (Keyboard::isKeyPressed(Keyboard::D))) {
        	herosprite.move(0.1*time, 0);
        	herosprite.setTextureRect(IntRect(0, 192, 96, 96));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Up)) || (Keyboard::isKeyPressed(Keyboard::W))) {
        	herosprite.move(0, -0.1*time);
        	herosprite.setTextureRect(IntRect(0, 288, 96, 96));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Down)) || (Keyboard::isKeyPressed(Keyboard::S))) {
        	herosprite.move(0, 0.1*time);
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
