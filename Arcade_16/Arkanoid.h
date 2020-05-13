#pragma once
#include "State.h"
#include <iostream>

constexpr int sizeM = 10, sizeN = 4;

class Arkanoid : public State {
public:
	Arkanoid(Font &f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);
	bool collision(Vector2f pos, Vector2f size);

	Texture b;
	Sprite ball;
	Button back;
	RectangleShape blocks[sizeM][sizeN];
	
	//ball
	RectangleShape left, right, top, bottom;
	bool xturn = 0, yturn = 0;
	int ballH = 30 * 0.7, ballW = ballH;
	int sizeX = 40, sizeY = 20;
	int offsetX = 30, offsetY = 40, spacing = 2;
	float speedx = 0.6, speedy = 0.5;
	Vector2f pos, prevPos;

	//the thing
	RectangleShape paddle;
	Vector2f pPos{ 130,420 };
	float pspeed = 0;
};


