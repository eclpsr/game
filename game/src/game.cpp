#include "SFML/Graphics.hpp"
using namespace sf;
int main()
{
    RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

    Texture herotexture; // ������ ������ Texture(��������)
    herotexture.loadFromFile("src/images/hero.png");// ��������� ��������

    Sprite herosprite; // ������ ������ Sprite(������)
    herosprite.setTexture(herotexture); // ������� � ���� ������ Texture (��������)
    herosprite.setTextureRect(IntRect(0,192,96,96));
    herosprite.setPosition(50, 25); // ����� ��������� ���������� ��������� �������

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(herosprite);
        window.display();
    }

    return 0;
}
