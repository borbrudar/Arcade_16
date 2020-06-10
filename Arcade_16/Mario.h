#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Mario {
public:
	void setup(Vector2f pos, Vector2f size, Texture& t);
	void draw(RenderWindow& window);
	void update(bool left, bool right);

	Vector2f pos;
	RectangleShape box;
	float speed = 0.5f;
};