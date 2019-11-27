#ifndef VIEW_H_
#define VIEW_H_
#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view; // объявили sfml объект "вид", который и является камерой

View setPlayerCoordinateForView(float x, float y) { // ф-ция для считывания координат игрока
	float tempX = x; float tempY = y; // считываем коорд игрока и проверяем их, чтобы убрать края
	if (x < 320) tempX = 320; // убираем из вида левую сторону
	if (y < 240) tempY = 240; // верхнюю сторону
	if (y > 554) tempY = 554; // нижнюю сторону
	view.setCenter(tempX, tempY); // следим за игроком, передавая его координаты камере. + 100 сместили камеру иксу вправо.
	return view;
}



#endif /* VIEW_H_ */
