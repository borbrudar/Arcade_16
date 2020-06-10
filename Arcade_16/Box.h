#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Box {
public:
	Box(Vector2f pos, Vector2f size, Texture& t) {
		box.setPosition(pos);
		box.setTexture(&t);
		box.setSize(size);
	};
	void draw(RenderWindow& window) {
		window.draw(box);
	}

	RectangleShape box;
};