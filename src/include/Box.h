#pragma once
#include <SFML/Graphics.hpp>
#include "BoxEnum.h"
#include "Manimation.h"

using namespace sf;

class Box {
public:
	Box(Vector2f pos, Vector2f size, Texture& t, Vector2f bSize, block_type type, int entity = 0);
	void draw(RenderWindow& window);
	void update(bool wiggle = 0);
	void off(float offX);

	Manimation box;
	int entity = 0;
	float offX = 0;
	bool hadEntity = 0;
	block_type type;
private:
	bool canWiggle = 0;
	int wiggling = 0;
	float speed = 1.f;
	Vector2f pos,oldPos;
};