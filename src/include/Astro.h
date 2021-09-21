#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

using namespace sf;

class Astro {
public:
	Astro() = default;
	Astro(int type, Texture &tex, Vector2f pos);
	void draw(RenderWindow& window);
	void update(float delta);

	CircleShape ast;
	float rot = 0, r = 0;
	Vector2f vel{ 0,0 };
};