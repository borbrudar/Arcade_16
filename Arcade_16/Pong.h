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
	int score1 = 0, score2 = 0;
	Text points;

	//player
	RectangleShape player, ball;
	bool up = 0, down = 0;
	float speedy = 0, csped = 1.0f;
	
	//ball
	float orgx = -1.f, orgy = 1.f;
	float bspedx = -1.f, bspedy = 1.f;
};