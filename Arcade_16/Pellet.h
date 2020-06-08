#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Pellet {
public:
	Pellet(Vector2f pos, bool super = 0) {
		setup(pos, super);
	}
	void setup(Vector2f pos, bool super = 0) {
		this->super = super;
		//normal
		if (!super) {
			pellet.setFillColor(Color::Yellow);
			pellet.setSize(Vector2f(5, 5));
			pellet.setOrigin(2.5, 2.5);
			pellet.setPosition(pos);
		}
		//super
		else {

		}
	}
	void draw(RenderWindow& window) {
		window.draw(pellet);
	}


	RectangleShape pellet;
	bool super = 0;
};