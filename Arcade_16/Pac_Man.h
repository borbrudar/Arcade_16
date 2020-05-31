#pragma once
#include "State.h"

class Pac_Man : public State {
public:
	Pac_Man(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

private:
	Button back;
};