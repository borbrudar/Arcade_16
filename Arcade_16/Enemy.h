#pragma once
#include <SFML/Graphics.hpp>
#include "Manimation.h"

using namespace sf;

class Enemy {
public:
	Enemy() = default;
	Enemy(Vector2f pos, Vector2f size,Vector2f tSize, Texture &t1);
	void setup(Vector2f pos, Vector2f size, Vector2f tSize, Texture &t1);
	void draw(RenderWindow& window);
	bool update(bool col, bool onScr = 0);

	bool onScreen = 0, groundTouch = 0, alive = 1;
	float speed = 0.5f, gravity = 1.5f;
	Manimation anim;
	RectangleShape death;
	Vector2f pos, prevPos;

	//death
	Clock dclock;
	float dtime = 0, dtimer = 0, ddelay = 1.0f;
};