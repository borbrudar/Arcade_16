#pragma once
#include <SFML/Graphics.hpp>
#include "Manimation.h"

using namespace sf;

class Entity {
public:
	Entity(Texture &t, Vector2f size, int maxSwap, Vector2f pos, Vector2f tSize, int type);
	void draw(RenderWindow& window);
	void update(Vector2f until);

	Manimation anim;
	Vector2f pos, orgPos;
	int type = 1;
};