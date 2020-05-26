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
	 
	RectangleShape player;
	bool up = 0, down = 0;
	float speedy = 0, csped = 0.5f;
};