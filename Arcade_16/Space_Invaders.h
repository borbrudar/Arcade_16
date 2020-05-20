#pragma once
#include "State.h"
#include "Animation.h"
#include <vector>

constexpr int invdM = 10, invdN = 6;
const float space_scale = 3.f;

class Space_Invaders : public State {
public:
	Space_Invaders(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

	std::vector < std::vector<Animation> > invaders;
	Button back;
	Sprite cannon;
	Texture c;

	int offsetX = 30, offsetY = 50;
	Vector2f size1{ 11,8 }, size2{ 8, 8 }, size3{ 12,8 }, cSize{ 13,8 };
	Vector2f spacing1{ 8,5 }, spacing2{ 17,5 }, spacing3{ 5,5 };
	float speedx = 0.2f, cannonx = 0.f;
};