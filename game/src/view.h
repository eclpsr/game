#ifndef VIEW_H_
#define VIEW_H_
#include <SFML/Graphics.hpp>

sf::View view; // объявили sfml объект "вид", который и является камерой

void getplayercoordinateforview(float x, float y) { // ф-ция для считывания координат игрока
	view.setCenter(x + 100, y); // следим за игроком, передавая его координаты камере. + 100 сместили камеру иксу вправо.
}




#endif /* VIEW_H_ */
