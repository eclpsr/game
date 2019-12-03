#include "SFML/Graphics.hpp"
#include <iostream>
#include "iostream"
#include "view.h"
#include "mission.h"
#include <cmath>
#include <vector>
#include <list>
#include "level.h"

using namespace sf;
class Entity {
public:
	std::vector<Object> obj; // Вектор объектов карты
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;

	Entity(Image &image, String Name, float X, float Y, int W, int H ){
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect() { //ф-ция получения прямоугольника. его коорд.размеры(шир, высот)
		return FloatRect(x, y, w, h); // эта ф-ция нужна для проверки столкновений
	}

	virtual void update(float time) = 0;
};

class Player:public Entity {
public:
	enum {left, right, up, down, jump, stay} state; // состояние объекта
	int playerScore;
	Player(Image &image, String Name, Level &lev, float X, float Y, int W, int H ) :Entity(image, Name, X, Y, W, H ){
	playerScore = 0; state = stay; obj = lev.GetAllObjects();
	if (name == "Player1"){
	sprite.setTextureRect(IntRect(4, 19, w, h));
	}
	}

void control(){
    if (Keyboard::isKeyPressed(Keyboard::Left)){
    	state = left;
    	speed = 0.1; // dir направление, speed скорость
    //	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам"
    //	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
    	//p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 96, 96, 96));
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)){
    	state = right;
    	speed = 0.1;
    //	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
    	//if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
    	//p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 192, 96, 96));
    }
    if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)){
    	state = jump;
    	dy = -0.6;
    	onGround = false; // прыгнули и сообщили что не на земле.
    //	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
    	//if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
    //	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 307, 96, 96));
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)){
    	state = down;
    	// CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
    	// if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
    	// p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 0, 96, 96));
    }
}

void checkCollisionWithMap(float Dx, float Dy) {
	for (int i = 0; i<obj.size(); i++)//проходимся по объектам
	if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
	{
	if (obj[i].name == "solid"){//если встретили препятствие (объект с именем solid)
	if (Dy>0)	{ y = obj[i].rect.top - h;  dy = 0; onGround = true; }
	if (Dy<0)	{ y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
	if (Dx>0)	{ x = obj[i].rect.left - w;}
	if (Dx<0)	{ x = obj[i].rect.left + obj[i].rect.width; }
	//}
	}
	}
}

void update(float time) {
	control();
	switch (state) {
	case right: dx = speed; break;
	case left: dx = -speed; break;
	case up: break; // состояние идти вверх (например по лестнице)
	case down: dx = 0; break;// состояние идти вниз (например по лестнице)
	case jump: break; // здесь может быть вывов анимации
	case stay: break; // и здесь тоже
	}
  x += dx*time;
  checkCollisionWithMap(dx, 0);
  y += dy*time;
  checkCollisionWithMap(0, dy);
  sprite.setPosition(x+w/2,y+h/2);
  if (health <= 0){ life = false;}
  if(!isMove){ speed = 0; }
  if(life){ setPlayerCoordinateForView(x, y); }
  dy = dy + 0.0015*time;
}
};

class Enemy :public Entity{
public:
Enemy(Image &image, String Name,Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H){
obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
if (name == "EasyEnemy"){
sprite.setTextureRect(IntRect(0, 0, w, h));
dx = 0.1;
}
}
void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
{
	for (int i = 0; i<obj.size(); i++)//проходимся по объектам
	if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
	{
	//if (obj[i].name == "solid"){//если встретили препятствие (объект с именем solid)
	if (Dy>0)	{ y = obj[i].rect.top - h;  dy = 0; onGround = true; }
	if (Dy<0)	{ y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
	if (Dx>0)	{ x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
	if (Dx<0)	{ x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
	//}
	}
}
void update(float time)
{
if (name == "EasyEnemy"){//для персонажа с таким именем логика будет такой
checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
x += dx*time;
sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
if (health <= 0){ life = false; }
}
}
};

int main() {
    RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
    view.reset(sf::FloatRect(0, 0, 640, 480)); // размер "вида" камеры при создании объекта вида камеры.

    std::list<Entity*> entities;//создаю список, сюда буду кидать объекты.например врагов.
    std::list<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка

    Level lvl; // создали экземпляр класса уровень
    lvl.LoadFromFile("src/map.tmx");

    Image heroImage;
    heroImage.loadFromFile("src/images/MilesTailsPrower.gif");

    Image easyEnemyImage;
   	easyEnemyImage.loadFromFile("src/images/shamaich.png");
   	easyEnemyImage.createMaskFromColor(Color(255, 0, 0));

       std::vector<Object> e = lvl.GetObjects("EasyEnemy");//все объекты врага на tmx карте хранятся в этом векторе
       for (int i = 0; i < e.size(); i++)//проходимся по элементам этого вектора(а именно по врагам)
       	entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 200, 97));//и закидываем в список всех наших врагов с карты

	Object player=lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);//передаем координаты прямоугольника player из карты в координаты нашего игрока

	Clock clock;
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
        p.update(time); //оживляем объект p класса Player с помощью времени sfml

        for (it = entities.begin(); it != entities.end();) {
        	Entity *b = *it;//для удобства, чтобы не писать (*it)->
        	b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
        	if (b->life == false){
        		it = entities.erase(it); delete b;  // if (entities.empty()) break;
        	}// если этот объект мертв, то удаляем его
        	else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
        }//для всех элементов списка(пока это только враги,но могут быть и пули к примеру) активируем ф-цию update

        for (it = entities.begin(); it != entities.end(); it++)//проходимся по эл-там списка
        {
        if ((*it)->getRect().intersects(p.getRect()))//если прямоугольник спрайта объекта пересекается с игроком
        {
        if ((*it)->name == "EasyEnemy"){//и при этом имя объекта EasyEnemy,то..
        if ((p.dy>0) && (p.onGround == false)) { (*it)->dx = 0; p.dy = -0.2; (*it)->health = 0; }//если прыгнули на врага,то даем врагу скорость 0,отпрыгиваем от него чуть вверх,даем ему здоровье 0
        else {
        p.health -= 5;	//иначе враг подошел к нам сбоку и нанес урон
        }
        }
        }
        }

        window.setView(view); // "оживляем" камеру в окне sfml
        window.clear(Color(77,83,140));
        lvl.Draw(window); // рисуем новую карту
        for (it = entities.begin(); it != entities.end(); it++){
        window.draw((*it)->sprite); //рисуем entities объекты (сейчас это только враги)
        }
        window.draw(p.sprite);
        window.display();
    }

    return 0;
}
