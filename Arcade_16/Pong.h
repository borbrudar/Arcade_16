#pragma once
#include "State.h"
#include <random>


using namespace sf;
class Pong : public State {
public:
	Pong(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);
	void aiMove();
	
	Button back;
	RectangleShape line;
	int score1 = 0, score2 = 0;
	Text points;

	//player
	RectangleShape player, ball, ai;
	std::vector<RectangleShape> b;
	Vector2f bSize{ 10,10 };
	Vector2f prevPos;
	bool up = 0, down = 0;
	float speedy = 0, csped = 1.0f;
	
	//ball
	float orgx = -0.8f, orgy = 0.8f;
	float tx, ty;
	float bspedx = -.8f, bspedy = .8f;

	float aiy = 0, aisped = 1.f;
	bool aiu = 0, aid = 0;
	float prob = 0.2f;
};