#pragma once
#include "State.h"
#include <iostream>
#include <time.h>

constexpr int M = 20, N = 10;

class Tetris : public State{
public:
	Tetris(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event &e);
	void checkBorder();
	void newPiece();

	Button back;
	Texture til;
	Sprite tiles;

	struct point { int x = 0, y = 0; } a[4], b[4];

	int field[M][N] = {};
	int shapes[7][4]{
		1,3,5,7, //I
		3,5,7,4, //T
		2,3,4,5, //O
		2,4,5,7, //Z
		3,4,5,6, //S
		3,5,6,7, //J
		2,4,6,7  //L
	};
	int dx = 0;
	bool rot = 0, fall = 1;
	Clock clock;
	float time = 0.f, timer = 0.f, delay = 0.2f, groundDelay = 0.7f; //adjust delay in update()
	int type = 0;
};