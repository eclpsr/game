#include "SFML/Graphics.hpp"
#include <iostream>
#include <sstream>
#include "map.h"
#include "view.h"
#include "mission.h"

using namespace sf;

class Player {
private:

public:
	float x, y;
	float w, h, dx, dy, speed;
	int dir, playerScore, health;
	bool life;
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
  interactionWithMap(); // вызываем функцию, отвечающую за взаимодействие с картой
  if (health <= 0){ life = false; speed = 0; }
}

float getplayercoordinateX(){
	return x;
}
float getplayercoordinateY(){
	return y;
}

void interactionWithMap() { //ф-ция взаимодействия с картой
	for(int i = y / 32; i < (y + h) / 32; i++) //проходимся по всей карте, то есть по всем квадратикам размера 32х32
			for (int j = x / 32; j < (x + w) / 32; j++){ // икс делим на 32, тем самым получаем левый квадратик
				if (TileMap[i][j] == '0'){ // если наш квадратик соответствует символу 0(стена), то проверяем "направление скорости" персонажа:
					if (dy>0){ // если мы шли вниз
						y = i * 32 - h; // то стопорим координату y персонажа. Сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта
					}
					if (dy<0){
						y = i * 32 + 32; // аналогично с ходьбюой вверх.
					}
					if(dx>0){
						x = j * 32 - w; // если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if(dx<0){
						x = j * 32 + 32; // аналогично идём влево
					}
				}
				if (TileMap[i][j] == 's'){ // если символ равен 's' (камень)
				    playerScore++; // если взял камень, инкремент переменной
					TileMap[i][j] = ' '; // убираем камень, типа взяли бонус.
				}
				if (TileMap[i][j] == 'f'){
				    health -= 40;
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'h'){
				    health += 20;
					TileMap[i][j] = ' ';
				}
			}
}

};

Player::Player(String F, float X, float Y, float W, float H){
dx=0; dy=0; speed=0; dir=0; playerScore=0; health = 100;
life = true;
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
    bool showMissionText = true;
    bool isMove = false; // переменная для щелчка мыши по спрайту
    float dX = 0; float dY = 0; // корректировка нажатия по х и y

    RenderWindow window(sf::VideoMode(1366, 768), "SFML works!", Style::Fullscreen);
    view.reset(sf::FloatRect(0, 0, 640, 480)); // размер "вида" камеры при создании объекта вида камеры.

    // *** MAP *** - B
    Image map_image; // объект изображения для карты
    map_image.loadFromFile("src/images/map.png"); // загружаем файл для карты
    Texture map; // текстура карты
    map.loadFromImage(map_image); // заряжаем текстуру картинкой
    Sprite s_map; // создаём спрайт для карты
    s_map.setTexture(map); // заливаем текстуру спрайтом
    // *** MAP *** - E

    randomMapGenerate(); // генерируем камешки

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

    float CurrentFrame = 0; // Хранит текущий кадр
	Clock clock; // создаем переменную времени, т.о. привязка ко времени(а не загруженности/мощности процессора).
	Clock gameTimeClock;
	int gameTime = 0;

	Player p("hero.png", 250, 250, 96.0, 96.0); // создаем объект p класса player

	int createObjectForMapTimer = 0; // переменная для генерации камней

    while (window.isOpen())
    {
    	float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
    	if(p.life) gameTime=gameTimeClock.getElapsedTime().asSeconds(); // игровое время пока игрок жив
    	clock.restart(); //перезагружает время
    	time = time/800; //скорость игры

    	Vector2i pixelPos = Mouse::getPosition(window); // забираем коорд курсора
    	Vector2f pos = window.mapPixelToCoords(pixelPos); // переводим их в игровые(уходим от коорд окна)

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == Event::KeyPressed)
            	if ((event.key.code) == Keyboard::Tab) {
            		switch(showMissionText){
            		case true: {
            			std::ostringstream playerHealthString;
            			playerHealthString << p.health;
            			std::ostringstream task;
            			task << getTextMission(getCurrentMission(p.getplayercoordinateX()));
            			text.setString("Health:" + playerHealthString.str() + "\n" + task.str());
            			showMissionText = false;
            			break;
            		}
            		case false: {
            			text.setString("");
            			showMissionText = true;
            			break;
            		}
            		}
            	}
            // *** Генерация камней *** - B
            createObjectForMapTimer += time;
            	if(createObjectForMapTimer>3000){
            		randomMapGenerate(); // генерация случ камней
            		createObjectForMapTimer = 0; // обнуляем таймер
            	}
            	// *** Генерация камней *** - E

            	// *** Передвижение объекта *** - B
            	if(event.type == Event::MouseButtonPressed)
            		if(event.key.code == Mouse::Left)
            			if (p.sprite.getGlobalBounds().contains(pos.x, pos.y)){
            				//std::cout << "isClicked!\n"; // выводим в консоль сообщение об этом
            				dX = pos.x - p.sprite.getPosition().x; // делаем разность между позицией курсора и спрайта для корректировки нажатия
            				dY = pos.y - p.sprite.getPosition().y; // тоже самое по игреку
            				isMove = true; // можем двигать спрайт
            			}

            	if (event.type == Event::MouseButtonReleased) // если отпустили клавишу
            		if (event.key.code == Mouse::Left) // а именно левую
            			isMove = false; // то не можем двигать спрайт
            			p.sprite.setColor(Color::White); // и даем ему прежний цвет
            	// *** Передвижение объекта *** - E
        }

        /// *** Двигаем спрайт мышью *** - B
        if(isMove){
        	p.sprite.setColor(Color::Green); // красим спрайт в зелёный
        	p.x = pos.x-dX; // двигаем спрайт по X
        	p.y = pos.y-dY; // двигаем спрайт по Y
        }
        /// *** Двигаем спрайт мышью *** - E

        if (p.life){
        if (Keyboard::isKeyPressed(Keyboard::Left)){
        	p.dir = 1; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам"
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 96, 96, 96));
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)){
        	p.dir = 0; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 192, 96, 96));
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)){
        	p.dir = 3; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 307, 96, 96));
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)){
        	p.dir = 2; p.speed = 0.1; // dir направление, speed скорость
        	CurrentFrame += 0.005*time; // служит для прохождения по "кадрам".
        	if(CurrentFrame > 3) CurrentFrame -= 3; // если пришли к третьему кадру - откидываемся назад
        	p.sprite.setTextureRect(IntRect(96*int(CurrentFrame), 0, 96, 96));
        }
        }
        sf::Vector2i localPosition = Mouse::getPosition(window); // заносим в вектор координаты мыши
        if (localPosition.x < 3) { view.move(-0.2*time, 0); }
        	if (localPosition.x > window.getSize().x-3) { view.move(0.2*time, 0); } // правый край-право
        	if (localPosition.y > window.getSize().y-3) { view.move(0, 0.2*time); } // нижний край - вниз
        	if (localPosition.y < 3) { view.move(0, -0.2*time); } // верхний край - вверх


        p.update(time); //оживляем объект p класса Player с помощью времени sfml

        window.setView(view); // "оживляем" камеру в окне sfml

        window.clear();

        // *** Рисуем карту *** - B
 if((getCurrentMission(p.getplayercoordinateX())) == 0){
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
 }

 if((getCurrentMission(p.getplayercoordinateX())) >= 1){
        for(int i = 0; i < HEIGHT_MAP; i++)
        	for(int j = 0; j < WIDTH_MAP; j++)
        	{
        		if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(64, 0, 32, 32));
        		if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32, 0, 32, 32));
        		if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(0, 0, 32, 32));
        		if (TileMap[i][j] == 'f') s_map.setTextureRect(IntRect(96, 0, 32, 32));
        		if (TileMap[i][j] == 'h') s_map.setTextureRect(IntRect(128, 0, 32, 32));
                s_map.setPosition(j * 32, i * 32); // по сути раскидывает квадратики, превращая в карту.
                window.draw(s_map);
        	}
 }
        // *** Риcуем карту *** - E
 	 	/// *** Выводим текст *** - B
        if (!showMissionText) {
        	text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//позиция всего этого текстового блока
        	s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//позиция фона для блока
        	window.draw(s_quest); window.draw(text);
        }
        /// *** Выводим текст *** - E
        window.draw(p.sprite);

        window.display();
    }

    return 0;
}
