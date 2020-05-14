#pragma once
#include "State.h"

class Space_Invaders : public State {
public:
	Space_Invaders(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

	Button back;
};