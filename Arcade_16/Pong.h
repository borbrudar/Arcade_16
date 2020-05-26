#pragma once
#include "State.h"


using namespace sf;
class Pong : public State {
public:
	Pong(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);
	 
	Button back;
	RectangleShape line;
	 
	RectangleShape player, ball;
	bool up = 0, down = 0;
	float speedy = 0, csped = 1.0f;
	float orgx = -0.5f, orgy = 0.5f;
	float bspedx = -0.5f, bspedy = 0.5f;
};