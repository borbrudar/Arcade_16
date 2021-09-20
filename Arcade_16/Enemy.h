#pragma once
#include <SFML/Graphics.hpp>
#include "Manimation.h"

using namespace sf;

class Enemy {
public:
	Enemy() = default;
	Enemy(Vector2f pos, Vector2f size,Vector2f tSize, Texture &t1, int type = 0);
	void setup(Vector2f pos, Vector2f size, Vector2f tSize, Texture &t1, int type = 0);
	void draw(RenderWindow& window);
	bool update(std::vector<int> etype, bool onScr = 0);
	void off(float offX);
	void boxUpdate();

	int type = 0, spin = 0;
	bool onScreen = 0, alive = 1, spinning = 0, showHitbox = 0;
	
	Manimation anim;
	std::vector<RectangleShape> enemybox;
	//y-axis is set with magic values for each number, so be careful
	Vector2f pos, prevPos;
private:
	//death
	Clock dclock;
	float dtime = 0, dtimer = 0, ddelay = 0.7f;
	RectangleShape death;

	float speed = 0.4f, gravity = 1.5f;
	bool groundTouch = 0, deathSet = 0;
	float offX = 0.f;
};