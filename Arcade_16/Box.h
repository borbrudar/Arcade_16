#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Box {
public:
	Box(Vector2f pos, Vector2f size, Texture& t);
	void draw(RenderWindow& window);

	RectangleShape box;
	int entity = 1;
};