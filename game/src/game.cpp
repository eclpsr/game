#include "SFML/Graphics.hpp"
#include <iostream>
#include "map.h"
#include "view.h"
using namespace sf;

class Player {
private: float x,y;
public:
	float w, h, dx, dy, speed;
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

float getplayercoordinateX(){
	return x;
}
float getplayercoordinateY(){
	return y;
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
// IntRect - приведение типов
}

int main()
{
    RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
    view.reset(sf::FloatRect(0, 0, 640, 480)); // размер "вида" камеры при создании объекта вида камеры.
    // *** MAP *** - B
    Image map_image; // объект изображения для карты
    map_image.loadFromFile("src/images/map.png"); // загружаем файл для карты
    Texture map; // текстура карты
    map.loadFromImage(map_image); // заряжаем текстуру картинкой
    Sprite s_map; // создаём спрайт для карты
    s_map.setTexture(map); // заливаем текстуру спрайтом
    // *** MAP *** - E

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
        if (Keyboard::isKeyPressed(Keyboard::Left)){
        	p.dir = 1; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам"
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 96, 96, 96));
        	getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)){
        	p.dir = 0; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 192, 96, 96));
        	getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)){
        	p.dir = 3; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 288, 96, 96));
        	getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)){
        	p.dir = 2; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 0, 96, 96));
        	getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
        	p.sprite.setColor(Color::Red);
        	p.sprite.setScale(2, 2);
        }

        p.update(time); //оживляем объект p класса Player с помощью времени sfml
        viewmap(time); // функция скроллига карты, передаём ей время sfml
        changeview();
        window.setView(view); // "оживляем" камеру в окне sfml

        window.clear();

        // *** Рисуем карту *** - B
        for(int i = 0; i < HEIGHT_MAP; i++)
        	for(int j = 0; j < WIDTH_MAP; j++)
        	{
        		if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
        		if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32, 0, 32, 32));
        		if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));
                s_map.setPosition(j * 32, i * 32); // по сути раскидывает квадратики, превращая в карту.
                window.draw(s_map);
        	}
        // *** Риcуем карту *** - E
        window.draw(p.sprite);
        window.display();
    }

    return 0;
}
