#pragma once
#include "State.h"
#include "Button.h"
#include <iostream>

class Menu : public State {
public:
	Menu(Font& f);
	void draw(RenderWindow &window);
	void update(Mouse &mouse, RenderWindow& window, state &gameState, Event &e);

	Button tetris, arkanoid, spaceinvaders, pong;
};