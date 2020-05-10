#pragma once
#include "State.h"

class Arkanoid : public State {
public:
	Arkanoid(Font &f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

	Button back;
};