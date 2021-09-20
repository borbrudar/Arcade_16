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
		pellet.setFillColor(Color::Yellow);
		pellet.setPosition(pos);
		//normal
		if (!super) {
			pellet.setSize(Vector2f(5, 5));
			pellet.setOrigin(2.5, 2.5);
		}
		//super
		else {
			pellet.setSize(Vector2f(8, 8));
			pellet.setOrigin(4, 4);
			pellet.setRotation(45.f);
		}
	}
	void draw(RenderWindow& window) {
		window.draw(pellet);
	}


	RectangleShape pellet;
	bool super = 0;
};