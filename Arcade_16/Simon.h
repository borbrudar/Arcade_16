#pragma once
#include "State.h"
#include <vector>
#include <iostream>

struct Box {
	void setup(Color col);
	void update();

	RectangleShape box;
	float oldr, oldg, oldb;
	float k = 0.7f;
};

class Simon : public State {
public:
	Simon(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

private:
	Button back;

	Vector2f bSize{ 200, 200 }, start{ 100,40 };
	std::vector<Box> boxes;
};