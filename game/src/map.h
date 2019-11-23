#ifndef MAP_H_
#define MAP_H_
#include <SFML\Graphics.hpp>
const int HEIGHT_MAP = 25;//размер карты высота.
const int WIDTH_MAP = 40;//размер карты ширина

sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0          f            h         h    0",
	"0   s                       s          0",
	"0                                 h    0",
	"0          f            h              0",
	"0                   s                  0",
	"0                                 h    0",
	"0          f            h              0",
	"0     s                                0",
	"0                    f      s          0",
	"0          f            h              0",
	"0                                 h    0",
	"0       s       s                      0",
	"0          f            h              0",
	"0                                 h    0",
	"0            s       f         s       0",
	"0          f            h              0",
	"0                                 h    0",
	"0                                      0",
	"0          f    s       h              0",
	"0                    f            h    0",
	"0          f                           0",
	"0                    f  h              0",
	"0          f                      h    0",
	"0000000000000000000000000000000000000000",
};

#endif /* MAP_H_ */
