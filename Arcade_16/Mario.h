#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include "Manimation.h"
#include <iostream>

using namespace sf;

class Mario {
public:
	void setup(Vector2f pos, Vector2f size, Texture& t, Vector2f tSize);
	void boxUpdate();
	void draw(RenderWindow& window);
	bool update(bool left, bool right, bool up, bool col, int type);
	const void resetJump();

	Manimation box;
	std::vector<RectangleShape> mariobox;
	float mariosp = 0.8f;
private:
	//physics
	Vector2f pos, prevPos;
	float gravity = 2.f, jump = gravity;
	bool groundTouch = 0, jumping = 0;

	//jumping clock
	Clock gclock, jdclock;
	float gtime = 0.f, gtimer = 0.f, gdelay = 0.3f, minDelay = 0.1f;
	float jdtime = 0.f, jdtimer = 0.f, jddelay = 0.2f;
	bool noDelay = 1;

	//animation
	bool prevR = 0, prevL = 0, prevU = 0;

};