#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>
#include <random>

using namespace sf;

class Astro {
public:
	void setup(int type, int size, Vector2f pos);
	void draw(RenderWindow& window);
	Sprite spr;
	CircleShape ast;
	Texture tex;
private:
	std::vector<RectangleShape> parts;
	Vector2f vel{ 0,0 };
	float rot = 0;
};