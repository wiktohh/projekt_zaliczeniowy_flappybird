#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "grafika.h"


struct Grafika {
		Texture bird;
		Texture pipe;
		Texture background;
		Texture gameover;
}grafika;

struct Bird {
		double v = 0;
		int frame = 0;
		Sprite sprite;
}bird;

enum GameState {
		waiting,
		started,
		gameover
}gamestate;

struct Game {
		int score{};
		int highscore{};
		int frames{};
		GameState gameState = waiting;
		Sprite background[3];
		Sprite gameover;
		Text pressC;
		Text scoreText;
		Text highscoreText;
		Font font;
}game;
