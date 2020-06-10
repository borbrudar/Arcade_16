#pragma once
#include "State.h"
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include <SFML/Audio.hpp>

struct BoxS {
	void setup(Color col);
	void bright();
	void update();

	RectangleShape box;
private:
	float oldr  = 0.f, oldg = 0.f, oldb = 0.f;
	const float k = 0.7f, mult = 3.f, sub = 0.05f;
};

class Simon : public State {
public:
	Simon(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);
	void inst(bool reset = 0);
	void music(int type);
	void stop();
private:
	Button back;
	int score = 0, highscore;
	std::fstream hs;
	Text scr;

	SoundBuffer rb,gb,bb,yb,eb;
	Sound red, green, blue, yellow, end;
	bool won = 1;
	Clock iclock;
	float itime = 0, itimer = 0, idelay = 0.9f;
	int ind = 0;

	std::vector<int> instructions;
	std::vector<int> moves;
	Vector2f bSize{ 200, 200 }, start{ 100,40 };
	std::vector<BoxS> boxes;
};