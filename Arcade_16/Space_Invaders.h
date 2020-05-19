#pragma once
#include "State.h"
#include "Animation.h"
#include <vector>

constexpr int invdM = 10, invdN = 4;
const float space_scale = 3.f;

class Space_Invaders : public State {
public:
	Space_Invaders(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

	std::vector < std::vector<Animation> > invaders;
	Button back;

	int offsetX = 30, offsetY = 50, spacing = 5;
	int sizeX = 11 , sizeY = 8;
	float speedx = 0.2f;
};