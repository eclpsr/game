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

    float CurrentFrame = 0; // Хранит текущий кадр

    while (window.isOpen())
    {
    	float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
    	clock.restart(); //перезагружает время
    	time = time/800; //скорость игры

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if ((Keyboard::isKeyPressed(Keyboard::Left)) || (Keyboard::isKeyPressed(Keyboard::A)))  {
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	herosprite.setTextureRect(IntRect(96*int(CurrentFrame), 96, 96, 96));
        	herosprite.move(-0.1*time, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::Right)) || (Keyboard::isKeyPressed(Keyboard::D))) {
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	herosprite.move(0.1*time, 0);
        	herosprite.setTextureRect(IntRect(96*int(CurrentFrame), 192, 96, 96));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Up)) || (Keyboard::isKeyPressed(Keyboard::W))) {
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	herosprite.move(0, -0.1*time);
        	herosprite.setTextureRect(IntRect(96*int(CurrentFrame), 288, 96, 96));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Down)) || (Keyboard::isKeyPressed(Keyboard::S))) {
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	herosprite.move(0, 0.1*time);
        	herosprite.setTextureRect(IntRect(96*int(CurrentFrame), 0, 96, 96));
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
        	herosprite.setColor(Color::Red);
        	herosprite.setScale(2, 2);
        }

        window.clear();
        window.draw(herosprite);
        window.display();
    }

    return 0;
}
