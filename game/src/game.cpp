#include "SFML/Graphics.hpp"
#include <iostream>
#include <sstream>
// #include "map.h"
#include "view.h"
#include "mission.h"
#include <cmath>
#include<vector>
#include<list>
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
	/* for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
	for (int j = x / 32; j<(x + w) / 32; j++)
	{
	if (TileMap[i][j] == '0')//если элемент наш тайлик земли? то
	{
	if (Dy>0){y = i * 32 - h;  dy = 0; onGround = true;}//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
	if (Dy<0){y = i * 32 + 32;  dy = 0;}//столкновение с верхними краями карты(может и не пригодиться)
	if (Dx>0){x = j * 32 - w;}//с правым краем карты
	if (Dx<0){x = j * 32 + 32;}// с левым краем карты
	}
	} */
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

/*float getplayercoordinateX(){
	return x;
}

float getplayercoordinateY(){
	return y;
} */

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
/* for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
for (int j = x / 32; j<(x + w) / 32; j++)
{
if (TileMap[i][j] == '0')//если элемент наш тайлик земли, то
{
if (Dy>0){ y = i * 32 - h; }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
if (Dy<0){ y = i * 32 + 32; }//столкновение с верхними краями карты(может и не пригодиться)
if (Dx>0){ x = j * 32 - w; dx = -0.1; sprite.scale(-1, 1); }//с правым краем карты
if (Dx<0){ x = j * 32 + 32; dx = 0.1; sprite.scale(-1, 1); }// с левым краем карты
}
} */
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
//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
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

    Level lvl; // создали экземпляр класса уровень
    lvl.LoadFromFile("src/map.tmx");
/*
    Image map_image; // объект изображения для карты
    map_image.loadFromFile("src/images/map.png"); // загружаем файл для карты
    Texture map; // текстура карты
    map.loadFromImage(map_image); // заряжаем текстуру картинкой
    Sprite s_map; // создаём спрайт для карты
    s_map.setTexture(map); // заливаем текстуру спрайтом
*/
    Image heroImage;
    heroImage.loadFromFile("src/images/MilesTailsPrower.gif");

    Image easyEnemyImage;
	easyEnemyImage.loadFromFile("src/images/shamaich.png");
	easyEnemyImage.createMaskFromColor(Color(255, 0, 0));

	Object player=lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	Object easyEnemyObject = lvl.GetObject("easyEnemy");//объект легкого врага на нашей карте.задаем координаты игроку в начале при помощи него
	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);//передаем координаты прямоугольника player из карты в координаты нашего игрока
	Enemy easyEnemy(easyEnemyImage, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 200, 97);//передаем координаты прямоугольника easyEnemy из карты в координаты нашего врага

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
        easyEnemy.update(time);
        window.setView(view); // "оживляем" камеру в окне sfml
        window.clear(Color(77,83,140));
        lvl.Draw(window); // рисуем новую карту
      /*
        for(int i = 0; i < HEIGHT_MAP; i++)
        	for(int j = 0; j < WIDTH_MAP; j++)
        	{
        		if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
        		if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32, 0, 32, 32));
        		if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));
        		if (TileMap[i][j] == 'f') s_map.setTextureRect(IntRect(96, 0, 32, 32));
        		if (TileMap[i][j] == 'h') s_map.setTextureRect(IntRect(128, 0, 32, 32));
                s_map.setPosition(j * 32, i * 32); // по сути раскидывает квадратики, превращая в карту.
                window.draw(s_map);
        	} */
        window.draw(easyEnemy.sprite);
        window.draw(p.sprite);
        window.display();
    }

    return 0;
}
