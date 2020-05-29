#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

using namespace sf;

class Astro {
public:
	Astro() = default;
	void setup(int type, int size, Vector2f pos);
	void draw(RenderWindow& window);

	CircleShape ast;
	float rot = 0;
	Vector2f vel{ 0,0 };
private:
	std::vector<RectangleShape> parts;
	Texture tex;
};