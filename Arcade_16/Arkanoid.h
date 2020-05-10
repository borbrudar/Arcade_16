#pragma once
#include "State.h"


constexpr int sizeM = 10, sizeN = 4;


class Arkanoid : public State {
public:
	Arkanoid(Font &f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

	Button back;
	RectangleShape blocks[sizeM][sizeN];
	int sizeX = 40, sizeY = 20;
	int offsetX = 30, offsetY = 40, spacing = 2;
};