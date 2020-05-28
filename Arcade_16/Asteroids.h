#pragma once
#include "State.h"
#include "Astro.h"
#include <vector>

using namespace sf;

const float ast_r = 2.f;

class astproj {
public:
	astproj(Vector2f pos, Vector2f vel);
	void draw(RenderWindow& window);
	bool isOffScreen();

	CircleShape projectile;
	Vector2f pos, vel;
};

class Asteroids : public State {
public:
	Asteroids(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

private:
	Texture sh;
	Sprite ship;
	Button back;

	//ship rotation
	const float ship_scale = 0.2f;
	float rot = 0, rots = 0.6f;
	bool left = 0, right = 0;

	//ship movement
	Vector2f vel{ 0,0 };
	float drag = 0.997f;
	bool move = 0;
	float maxsped = 1.5f;

	//pew pew pew
	bool shoot = 0;
	std::vector<astproj> bullets;
	Clock sClock;
	float stime = 0, stimer = 0, sdelay = 0.2f;
	float bulsped = 1.f;

	//actual asteroids
	Astro test;
};