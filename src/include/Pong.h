#pragma once
#include "State.h"
#include <random>
#include <SFML/Audio.hpp>


using namespace sf;
class Pong : public State {
public:
	Pong(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e, float delta);
	void aiMove();

private:	
	Button back;
	RectangleShape line;
	int score1 = 0, score2 = 0;
	Text points;
	float prob = 0.2f;

	//sound
	SoundBuffer col, death;
	Sound sound;
	//player
	RectangleShape player, ball, ai;
	std::vector<RectangleShape> b;
	Vector2f bSize{ 10,10 };
	Vector2f prevPos;
	Vector2f bpos[2] = { Vector2f(0, scrWidth / 2), Vector2f(scrWidth / 2, scrHeight - bSize.y) };
	bool up = 0, down = 0;
	float speedy = 0, csped = 800.0f;
	
	//ball
	float orgx = -20.f, orgy = 20.f;
	float orgy2 = -20.f, orgx2 = 20.f;
	float startSpeed = 600.f;
	float tx = 0, ty = 0;
	float bspedx = -startSpeed, bspedy = startSpeed;
	Vector2f ballPos;

	float aiy = 0, aisped = 800.f;
	bool aiu = 0, aid = 0;
};