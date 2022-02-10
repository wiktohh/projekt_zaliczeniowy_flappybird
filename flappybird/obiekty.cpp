#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <ctime>
#include "obiekty.h"



game.statusGry = waiting;

grafika.tlo.loadFromFile("../grafika/background.png");
grafika.rura.loadFromFile("../grafika/pipe.png");
grafika.gameover.loadFromFile("../grafika/gameover.png");
grafika.ptak.loadFromFile("../grafika/flappy1.png");


ptak.sprite.setPosition(250, 300);
ptak.sprite.setScale(2, 2);


game.font.loadFromFile("../czcionka/flappy.ttf");


game.tlo[0].setTexture(grafika.tlo);
game.tlo[1].setTexture(grafika.tlo);
game.tlo[2].setTexture(grafika.tlo);
game.tlo[0].setScale(1.3, 2.5);
game.tlo[1].setScale(1.3, 2.5);
game.tlo[2].setScale(1.3, 2.5);
game.tlo[1].setPosition(333, 0);
game.tlo[2].setPosition(666, 0);

game.pressC.setString("Press C to continue");
game.pressC.setFont(game.font);
game.pressC.setFillColor(Color::Black);
game.pressC.setCharacterSize(50);
game.pressC.setOrigin(game.pressC.getLocalBounds().width / 2, 0);
game.pressC.setPosition(500, 250);

game.wynikText.setFont(game.font);
game.wynikText.setCharacterSize(50);
game.wynikText.setFillColor(Color::White);
game.wynikText.move(30, 0);

game.najwyzszyWynikText.setFont(game.font);
game.najwyzszyWynikText.setCharacterSize(30);
game.najwyzszyWynikText.setFillColor(Color::White);
game.najwyzszyWynikText.move(30, 60);