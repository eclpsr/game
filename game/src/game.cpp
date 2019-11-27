#include "SFML/Graphics.hpp"
#include <iostream>
#include <sstream>
#include "map.h"
#include "view.h"
#include "mission.h"
#include <cmath>

using namespace sf;

class Player {
private:

public:
	float x, y;
	float w, h, dx, dy, speed;
	int playerScore, health;
	bool life;
	bool isMove, isSelect; // добавили переменные состояния движения и выбора объекта
	bool onGround; // на земле?
	enum {left, right, up, down, jump, stay} state; // состояние объекта
	String File; // файл с расширением
	Image image; // sfml изображение
	Texture texture; // sfml текстура
	Sprite sprite; // sfml спрайт
Player(String F, float X, float Y, float W, float H);

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
    	dy = -0.5;
    	onGround = false; // прыгнули и сообщили что не на земле.
    //	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
    	//if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
    //	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 307, 96, 96));
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)){
    	state = down;
    	speed = 0.1;
    	// CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
    	// if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
    	// p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 0, 96, 96));
    }
}

void checkCollisionWithMap(float Dx, float Dy) {
	for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
	for (int j = x / 32; j<(x + w) / 32; j++)
	{
	if (TileMap[i][j] == '0')//если элемент наш тайлик земли? то
	{
	if (Dy>0){ y = i * 32 - h;  dy = 0; onGround = true; }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
	if (Dy<0){y = i * 32 + 32;  dy = 0;}//столкновение с верхними краями карты(может и не пригодиться)
	if (Dx>0){x = j * 32 - w;}//с правым краем карты
	if (Dx<0){x = j * 32 + 32;}// с левым краем карты
	} else {onGround=false;}
	}
}

void update(float time) {
// Функция "оживления" объекта класса. update - обновление. Принимает в себя время SFML,
// Вследствии чего работает бесконечно, давая персонажу движение.
	control();
	switch (state) {
	case right: dx = speed; break;
	case left: dx = -speed; break;
	case up: break; // состояние идти вверх (например по лестнице)
	case down: break;// состояние идти вниз (например по лестнице)
	case jump: break; // здесь может быть вывов анимации
	case stay: break; // и здесь тоже
	}
  x += dx*time;
  checkCollisionWithMap(dx, 0);
  y += dy*time;
  checkCollisionWithMap(0, dy);
  if(!isMove) speed = 0;
  sprite.setPosition(x+w/2,y+h/2);
  if (health <= 0){ life = false;}
  if(!onGround) { dy = dy + 0.0015*time;} // притяжение к земле
  dy = dy + 0.0015*time; //checkCollisionWithMap(0, dy);
}

float getplayercoordinateX(){
	return x;
}

float getplayercoordinateY(){
	return y;
}

};

Player::Player(String F, float X, float Y, float W, float H){
dx=0; dy=0; speed=0; playerScore=0; health = 100;
life = true; isMove = false; isSelect = false; onGround = false; state = stay;
File = F; //имя файла+расширение
w = W; h = H; // высота и ширина
image.loadFromFile("src/images/" + File); // изображение
image.createMaskFromColor(Color(0, 0, 255)); //убираем темно-синий цвет
texture.loadFromImage(image); // загружаем изображение в текстуру
sprite.setTexture(texture); //заливаем спрайт текстурой
x = X; y = Y; //координата появления спрайта
sprite.setTextureRect(IntRect(0, 134, w, h)); //Задаем спрайту один прямоугольник для вывода одного льва, а не кучи сразу.
// IntRect - приведение типов
sprite.setOrigin(w/2, h/2);
}

int main()
{
    bool showMissionText = true;
    bool isMove = false; // переменная для щелчка мыши по спрайту
    float dX = 0; float dY = 0; // корректировка нажатия по х и y
    int tempX = 0; // временная кооорд. Х. Снимаем её после нажатия прав клав мыши
    int tempY = 0; // коорд Y
    float distance = 0; // это расстояие от объекта до тыка курсора

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

    // *** TEXT *** - B
    Font font;
    font.loadFromFile("src/fonts/Helvetica.otf");
    Text text("", font, 20);
    text.setColor(sf::Color::Black);
    Text text2("", font, 20);
    text2.setColor(sf::Color::Black);
    // *** TEXT *** - E

    // *** MISSION *** - B
    Image quest_image;
    	quest_image.loadFromFile("src/images/missionbg.jpg");
    	quest_image.createMaskFromColor(Color(0, 0, 0));
    	Texture quest_texture;
    	quest_texture.loadFromImage(quest_image);
    	Sprite s_quest;
    	s_quest.setTexture(quest_texture);
    	s_quest.setTextureRect(IntRect(0, 0, 340, 510));
    	s_quest.setScale(0.6f, 0.6f);
    // *** MISSION *** - E

	Clock clock; // создаем переменную времени, т.о. привязка ко времени(а не загруженности/мощности процессора).
	Clock gameTimeClock;

	Player p("hero.png", 250, 500, 96, 54); // создаем объект p класса player

    while (window.isOpen())
    {
    	float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
    	// if(p.life) gameTime=gameTimeClock.getElapsedTime().asSeconds(); // игровое время пока игрок жив
    	clock.restart(); //перезагружает время
    	time = time/800; //скорость игры

    	Vector2i pixelPos = Mouse::getPosition(window); // забираем коорд курсора
    	Vector2f pos = window.mapPixelToCoords(pixelPos); // переводим их в игровые(уходим от коорд окна)

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

        	// *** Передвижение объекта *** - B
        	if(event.type == Event::MouseButtonPressed)
        		if(event.key.code == Mouse::Left)
        			if (p.sprite.getGlobalBounds().contains(pos.x, pos.y)){
        				p.sprite.setColor(Color::Green); // красим спрайт в зелёный цвет
        				p.isSelect = true;
        			}

            	if (p.isSelect)
            		if (event.type == Event::MouseButtonPressed)
            			if (event.key.code == Mouse::Right){
            				p.isMove = true;
            				p.isSelect = false;
            				p.sprite.setColor(Color::White);
            				tempX = pos.x;
            				tempY = pos.y;
            		        float dX = pos.x - p.x;  // вектор, колинеарный прямой, которая пересекает спрайт и курсор
            		        float dY = pos.y - p.y; // он же, координата y
            		        float rotation = (atan2(dY, dX)) * 180 / 3.14159265; // получаем угол в радианах и переводим его в градусы
            		        std:: cout << rotation << "\n"; //  смотрим на градусы в консольке
            		        p.sprite.setRotation(rotation); // поворачиваем спрайт на эти градусы
            			}
        }

        if(p.isMove){
        	distance = sqrt((tempX - p.x)*(tempX - p.x) + (tempY - p.y)*(tempY - p.y));
        	if(distance > 2){
        		p.x += 0.1*time*(tempX - p.x) / distance; // идем по иксу с помощью вектора нормали
        		p.y += 0.1*time*(tempY - p.y) / distance; // идем по игреку так же
        	}
        	else { p.isMove = false; }
        }

        if (p.life){
        	getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
        }

        p.update(time); //оживляем объект p класса Player с помощью времени sfml

        window.setView(view); // "оживляем" камеру в окне sfml

        window.clear();

        // *** Рисуем карту *** - B

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
        	}

        window.draw(p.sprite);
        window.display();
    }

    return 0;
}
