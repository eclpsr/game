#include "SFML/Graphics.hpp"
#include <iostream>
using namespace sf;

class Player {
public:
	float x, y, w, h, dx, dy, speed;
	int dir; // направление игрока
	String File; // файл с расширением
	Image image; // sfml изображение
	Texture texture; // sfml текстура
	Sprite sprite; // sfml спрайт
Player(String F, float X, float Y, float W, float H);

void update(float time) {
// Функция "оживления" объекта класса. update - обновление. Принимает в себя время SFML,
// Вследствии чего работает бесконечно, давая персонажу движение.
	switch (dir) {
	case 0: dx = speed; dy = 0; break; // по х - задаем положительную скорость, по у - зануляем, получаем ход вправо.
	case 1: dx = -speed; dy = 0; break; // по х - задаем отрицательную скорость, по у - зануляем, получаем ход влево.
	case 2: dx = 0; dy = speed; break; // по х - задаем нулевое значение, по у - положительное, получаем ход вниз.
	case 3: dx = 0; dy = -speed; break; // по х - задаем нулевое значение, по у - положительное, получаем ход вверх.
	}
  x += dx*time;
  y += dy*time;
  speed = 0;
  sprite.setPosition(x,y);
}

};

Player::Player(String F, float X, float Y, float W, float H){
dx=0; dy=0; speed=0; dir=0;
File = F; //имя файла+расширение
w = W; h = H; // высота и ширина
image.loadFromFile("src/images/" + File); // изображение
image.createMaskFromColor(Color(41, 33, 59)); //убираем темно-синий цвет
texture.loadFromImage(image); // загружаем изображение в текстуру
sprite.setTexture(texture); //заливаем спрайт текстурой
x = X; y = Y; //координата появления спрайта
sprite.setTextureRect(IntRect(0, 0, w, h)); //Задаем спрайту один прямоугольник для вывода одного льва, а не кучи сразу.
// IntRect - приведение типов.
}

int main()
{
    RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

    float CurrentFrame = 0; // Хранит текущий кадр
	Clock clock; // создаем переменную времени, т.о. привязка ко времени(а не загруженности/мощности процессора).

	Player p("hero.png", 250, 250, 96.0, 96.0); // создаем объект p класса player

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
        	p.dir = 1; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 96, 96, 96));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Right)) || (Keyboard::isKeyPressed(Keyboard::D))) {
        	p.dir = 0; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 192, 96, 96));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Up)) || (Keyboard::isKeyPressed(Keyboard::W))) {
        	p.dir = 3; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 288, 96, 96));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Down)) || (Keyboard::isKeyPressed(Keyboard::S))) {
        	p.dir = 2; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 0, 96, 96));
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
        	p.sprite.setColor(Color::Red);
        	p.sprite.setScale(2, 2);
        }

        p.update(time); //оживляем объект p класса Player с помощью времени sfml

        window.clear();
        window.draw(p.sprite);
        window.display();
    }

    return 0;
}
