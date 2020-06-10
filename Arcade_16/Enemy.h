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
	void update(bool col);

	bool onScreen = 1, groundTouch = 0;
	float speed = 0.5f, gravity = 1.5f;
	Manimation anim;
	Vector2f pos, prevPos;
};