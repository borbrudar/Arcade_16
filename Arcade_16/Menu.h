#pragma once
#include "State.h"
#include "Button.h"
#include <iostream>

class Menu : public State {
public:
	Menu(Font &f) {
		std::string text1;
		text1.assign("1.Tetris");
		button.setup(f, text1, Color::Green, Vector2f(350, 50), Vector2f(70,50));
	}

	void draw(RenderWindow &window);
	void update(Mouse &mouse, RenderWindow& window, state &gameState, Event &e);

	Button button;
};