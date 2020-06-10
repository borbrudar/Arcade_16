#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"

using namespace sf;

class Mario {
public:
	void setup(Vector2f pos, Vector2f size, Texture& t);
	void draw(RenderWindow& window);
	bool update(bool left, bool right, bool up, bool col);

	RectangleShape box;
	float mariosp = 0.8f;
private:
	Vector2f pos, prevPos;
	float gravity = 1.5f, jump = 3.f;
	bool groundTouch = 0, jumping = 0;

	Clock gclock;
	float gtime = 0.f, gtimer = 0.f, gdelay = 0.4f;
};