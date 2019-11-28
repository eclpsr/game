#ifndef VIEW_H_
#define VIEW_H_
#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view; // объявили sfml объект "вид", который и является камерой

void setPlayerCoordinateForView(float x, float y) { // ф-ция для считывания координат игрока
	float tempX = x; float tempY = y; // считываем коорд игрока и проверяем их, чтобы убрать края
	//if (x < 320) tempX = 320; // убираем из вида левую сторону
	//if (y < 240) tempY = 240; // верхнюю сторону
	if (y > 720) tempY = 720; // нижнюю сторону
	view.setCenter(tempX, tempY); // следим за игроком, передавая его координаты камере. + 100 сместили камеру иксу вправо.
}



#endif /* VIEW_H_ */
