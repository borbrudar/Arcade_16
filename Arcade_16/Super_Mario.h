#pragma once
#include "State.h"
#include "Mario.h"
#include "Box.h"
#include "Enemy.h"
#include "Entity.h"
#include "BoxEnum.h"

class Super_Mario : public State {
public:
	Super_Mario(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

	Button back;

	//level
	Image lvl;
	Texture gr, mr, br;

	std::vector<Box> boxes;
	std::vector<Box> blocks;
	std::vector<Box> coins_;
	Vector2f off{ 0,20 }, tSize, bSize{ 16,16 };
	float offX = 0.f;
	int coins = 0;

	//mario
	Mario mario;
	bool left = 0, right = 0, up = 0;
	Vector2f mSize{ 17,16 };

	//enemies
	std::vector<Enemy> enemies;
	Vector2f e1Size{ 16,16 }, e2Size{ 16,24 };
	Texture en, en2;

	//entity
	std::vector<Entity> entities;
	Texture tits;
	Vector2f cSize{ 10,14 };
};