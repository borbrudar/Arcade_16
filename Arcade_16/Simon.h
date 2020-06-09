#pragma once
#include "State.h"
#include <vector>

class Simon : public State {
public:
	Simon(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

private:
	Button back;

	Vector2f bSize{ 200, 200 }, start{ 100,40 };
	std::vector<RectangleShape> boxes;
};