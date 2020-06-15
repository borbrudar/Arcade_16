#pragma once
#include <SFML/Graphics.hpp>
#include "Manimation.h"

using namespace sf;

class MP {
public:
	void setup(Vector2f pos, Texture& t1, Vector2f pSize, Vector2f tSize);
	void draw(RenderWindow& window);
	void update(std::vector<int> ptype);
	void boxUpdate();


	std::vector<RectangleShape> projbox;
	Manimation box;
	Vector2f pos, prevPos;

	Clock clock;
	float time = 0.f, timer = 0.f, delay = 0.1f;
	float speedx = 0.6f, speedy = 1.5f;
	bool alive = 1;
};