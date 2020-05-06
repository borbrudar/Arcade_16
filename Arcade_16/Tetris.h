#pragma once
#include "State.h"

class Tetris : public State{
public:
	Tetris(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event &e);

	Button back;
	Texture til;
	Sprite tiles;
};