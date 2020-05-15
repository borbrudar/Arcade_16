#pragma once
#include "State.h"
#include <vector>

constexpr int invdM = 10, invdN = 5;

class Space_Invaders : public State {
public:
	Space_Invaders(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

	std::vector < std::vector<RectangleShape> > invaders;
	Button back;

	int offsetX = 30, offsetY = 40, spacing = 5;
	int sizeX = 30, sizeY = 30;
};