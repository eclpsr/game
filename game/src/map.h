#ifndef MAP_H_
#define MAP_H_
#include <SFML\Graphics.hpp>
const int HEIGHT_MAP = 25;//размер карты высота.
const int WIDTH_MAP = 40;//размер карты ширина

sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0          f            h         h    0",
	"0                                      0",
	"0                                 h    0",
	"0          f            h              0",
	"0                                      0",
	"0                                 h    0",
	"0          f            h              0",
	"0                                      0",
	"0                    f                 0",
	"0          f            h              0",
	"0                                 h    0",
	"0                                      0",
	"0          f            h              0",
	"0                                 h    0",
	"0                    f                 0",
	"0          f            h              0",
	"0                                 h    0",
	"0                                      0",
	"0          f            h              0",
	"0                    f            h    0",
	"0          f                           0",
	"0                    f  h              0",
	"0          f                      h    0",
	"0000000000000000000000000000000000000000",
};

void randomMapGenerate(){
	int randomElementX = 0; // случайный элемент по горизонтали
	int randomElementY = 0; // случайный элемент по вертикали
	srand(time(0)); // рандом
	int countStone = 2; // количество камней

	while(countStone>0){
		randomElementX = 1 + rand() % (WIDTH_MAP - 1); // рандомное по иксу от 1 до ширина карты - 1, чтобы не получать числа бордюра карты
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1); // по игреку так же
		if(TileMap[randomElementY][randomElementX] == ' ') { // если встретили символ пробел,
			TileMap[randomElementY][randomElementX] = 's';
			countStone--;
		}
	}
}

#endif /* MAP_H_ */
