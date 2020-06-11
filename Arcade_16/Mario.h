#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"

using namespace sf;

class Mario {
public:
	void setup(Vector2f pos, Vector2f size, Texture& t);
	void boxUpdate();
	void draw(RenderWindow& window);
	bool update(bool left, bool right, bool up, bool col, int type);

	RectangleShape box;
	std::vector<RectangleShape> mariobox;
	float mariosp = 0.8f;
private:
	Vector2f pos, prevPos;
	float gravity = 1.6f, jump = gravity;
	bool groundTouch = 0, jumping = 0, firstJump = 1;

	Clock gclock;
	float gtime = 0.f, gtimer = 0.f, gdelay = 0.3f;
};