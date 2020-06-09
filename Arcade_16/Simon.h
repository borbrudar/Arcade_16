#pragma once
#include "State.h"
#include <vector>
#include <iostream>
#include <random>

struct Box {
	void setup(Color col);
	void bright();
	void update();

	RectangleShape box;
private:
	float oldr  = 0.f, oldg = 0.f, oldb = 0.f;
	const float k = 0.7f, sub = 0.1f;
};

class Simon : public State {
public:
	Simon(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);
	void inst(bool reset = 0);
private:
	Button back;
	bool won = 0;

	std::vector<int> instructions;
	std::vector<int> moves;
	Vector2f bSize{ 200, 200 }, start{ 100,40 };
	std::vector<Box> boxes;
};