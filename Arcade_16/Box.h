#pragma once
#include <SFML/Graphics.hpp>
#include "BoxEnum.h"
#include "Manimation.h"

using namespace sf;

class Box {
public:
	Box(Vector2f pos, Vector2f size, Texture& t, Vector2f bSize, block_type type);
	void draw(RenderWindow& window);
	void update(bool wiggle = 0);

	Manimation box;
	int entity = 0;
private:
	bool canWiggle = 0, hadEntity = 0;
	int wiggling = 0;
	float speed = 1.f;
	Vector2f oldPos;
	block_type type;
};