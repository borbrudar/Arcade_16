#pragma once
#include "State.h"
#include "Mario.h"
#include "Box.h"
#include "Enemy.h"

class Super_Mario : public State {
public:
	Super_Mario(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

	Button back;

	//level
	Image lvl;
	Texture gr, mr;
	std::vector<Box> boxes;
	std::vector<Box> blocks;
	Vector2f off{ 0,20 };
	float offX = 0.f;

	//mario
	Mario mario;
	bool left = 0, right = 0, up = 0;
	std::vector<RectangleShape> mariobox;

	//enemies
	std::vector<Enemy> enemies;
	Vector2f eSize{ 16,16 };
	Texture en;
};