#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include "Manimation.h"

using namespace sf;

class Mario {
public:
	void setup(Vector2f pos, Vector2f size, Texture& t, Vector2f tSize);
	void boxUpdate();
	void draw(RenderWindow& window);
	bool update(bool left, bool right, bool up, bool col, int type);

	Manimation box;
	std::vector<RectangleShape> mariobox;
	float mariosp = 0.8f;
private:
	//physics
	Vector2f pos, prevPos;
	float gravity = 1.6f, jump = gravity;
	bool groundTouch = 0, jumping = 0, firstJump = 1;

	//clock
	Clock gclock;
	float gtime = 0.f, gtimer = 0.f, gdelay = 0.3f;

	//animation
	bool prevR = 0, prevL = 0, prevU = 0;
};