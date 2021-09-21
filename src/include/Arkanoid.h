#pragma once
#include "State.h"
#include <iostream>

constexpr int sizeM = 14, sizeN = 4;

class Arkanoid : public State {
public:
	Arkanoid(Font &f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e, float delta);
	bool collision(Vector2f pos, Vector2f size);

	Texture b;
	Sprite ball;
	Button back;
	RectangleShape blocks[sizeM][sizeN];
	
	//ball
	RectangleShape left, right, top, bottom;
	bool xturn = 0, yturn = 0;
	float ballH = 30 * 0.7f, ballW = ballH;
	float sizeX = 40, sizeY = 20;
	int offsetX = 30, offsetY = 40, spacing = 2;
	float speedx = 800.f, speedy = 600.f;
	Vector2f pos, prevPos;

	//the thing
	RectangleShape paddle;
	Vector2f pPos{ 250,420 };
	float pspeed = 750.f;

	//game
	bool gameOver = 0;
	bool l = 0, r = 0;
};


