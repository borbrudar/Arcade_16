#pragma once
#include "State.h"

class Simon : public State {
public:
	Simon(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

	Button back;
};