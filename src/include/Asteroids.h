#pragma once
#include "State.h"
#include "Astro.h"
#include <vector>
#include <iostream>
#include <fstream>

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
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e, float delta);

private:
	Vector2f pos();
	void shipCol(std::vector<Astro>& vec);

	Texture sh;
	Sprite ship;
	Button back;
	long score = 0, highscore;
	std::fstream hs;
	Text scr;
	bool gameOver = 0;

	//ship rotation
	const float ship_scale = 0.2f;
	float rot = 0, rots = 300.f;
	bool left = 0, right = 0;

	//ship movement
	Vector2f vel{ 0,0 };
	float drag = 0.9999f;
	bool move = 0;
	float maxsped = 0.17f;
	float speed = .12f;

	//pew pew pew
	bool shoot = 0;
	std::vector<astproj> bullets;
	Clock sClock;
	float stime = 0, stimer = 0, sdelay = 0.2f;
	float bulsped = 1.f;

	//actual asteroids
	std::vector<Astro> big;
	std::vector<Astro> medium;
	std::vector<Astro> small;
	std::vector<Texture> bigt,medt, smat;
	Clock aClock;
	float atimer = 0, atime = 0, adelay = 4.0f;
	int bmax = 4, mmax = 8, smax = 12;

	//ast-ship collision
	CircleShape col;
};