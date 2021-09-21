#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include "Manimation.h"
#include <iostream>

using namespace sf;

class Mario {
public:
	void setup(Vector2f pos, std::vector<Vector2f> msize, std::vector<Texture>& t, Vector2f tSize);
	void boxUpdate();
	const void boxResize();
	void draw(RenderWindow& window);
	bool update(bool left, bool right, bool up, bool col, std::vector<int> type, float delta, bool sprint = 0);
	bool update();

	Manimation box;
	std::vector<RectangleShape> mariobox;
	float mariosp = 200.f, sprintsp = 1.6f;
	bool showHitbox = 0, big = 0, shiny = 0, canShoot = 0, prevDir = 1, alive = 1, op = 0, groundTouch = 0;
private:
	//op clock
	Clock opc;
	float opt = 0.f, optr = 0.f, opd = 3.5f;
	//physics
	Vector2f pos, prevPos;
	float gravity = 400.f, jump = gravity;
	bool jumping = 0, canLeft = 1, canRight = 1, checkBig = 0, checkShiny = 0;

	//shooting clock
	Clock sclock;
	float stime = 0.f, stimer = 0.f, sdelay = 0.4f;

	//jumping clock
	Clock gclock, jdclock;
	float gtime = 0.f, gtimer = 0.f, gdelay = 0.3f, minDelay = 0.12f;
	float jdtime = 0.f, jdtimer = 0.f, jddelay = 0.2f;
	bool noDelay = 1;

	//animation
	bool prevR = 0, prevU = 0, prevL = 0; //dont be fooled, prevU is actually for standing after landing
	std::vector<Texture> tex;
	std::vector<Vector2f> mSize;
	Vector2f tSize;
};