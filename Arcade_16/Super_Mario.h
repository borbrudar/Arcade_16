#pragma once
#include "State.h"

class Ground {
public:
	Ground(Vector2f pos, Vector2f size, Texture& t) {
		box.setPosition(pos);
		box.setTexture(&t);
		box.setSize(size);
	};
	void draw(RenderWindow& window) {
		window.draw(box);
	}
	RectangleShape box;
};

class Super_Mario : public State {
public:
	Super_Mario(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

	Button back;

	Image lvl;
	Texture gr;
	std::vector<Ground> ground;
	Vector2f off{ 0,20 };
};