#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Mario {
public:
	void setup(Vector2f pos, Vector2f size, Texture& t);
	void draw(RenderWindow& window);
	void update(bool left, bool right, bool up, bool col);

	RectangleShape box;
private:
	Vector2f pos, prevPos;
	float speed = 0.5f, gravity = 1.5f, jump = 2.f;
	bool groundTouch = 0, jumping = 0;

	Clock gclock;
	float gtime = 0.f, gtimer = 0.f, gdelay = 0.2f;
};