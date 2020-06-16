#pragma once
#include <SFML/Graphics.hpp>
#include "Manimation.h"

using namespace sf;

class MP {
public:
	void setup(Vector2f pos, Texture& t1, Vector2f pSize, Vector2f tSize);
	void draw(RenderWindow& window);
	bool update(std::vector<int> ptype);
	void boxUpdate();
	void off(int x);

	std::vector<RectangleShape> projbox;
	Manimation box;
	Vector2f pos, prevPos;
	int oddX = 0;
private:
	Clock clock;
	float time = 0.f, timer = 0.f, delay = 0.1f;
	float speedx = 0.6f, speedy = 1.5f;

	int offX = 0;
};