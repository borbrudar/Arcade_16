#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <random>
#include <cmath>
#include <iostream>

using namespace sf;

class Ghost {
public:
	void setup(int type, Vector2f pos, Vector2f size, Vector2f start, std::vector<std::vector<int>> field);
	void draw(RenderWindow& window);
	//blinky and clyde
	void update(Vector2f player, bool fright = 0);
	//pinky
	void update(Vector2f player, float rot, bool fright = 0);
	//inky
	void update(Vector2f player, float rot, Vector2f blinky, bool fright = 0);
	//call when collision and other stuff
	void die();

	void reset();

	Vector2f pos;
	Animation animation;
	bool alive = 1;
private:
	void findPath(Vector2i target, Vector2i curPos);
	void random(Vector2i curPos);
	bool move(bool fright = 0);
	

	std::vector<int> beggining;
	int st = 1;
	std::vector<std::vector<int>> field;
	int type = 0, instruction = -1;
	float speed = 1.3f;
	bool  beg = 1;

	float distTraveled = 0.f;
	Vector2i prevPos;
	Vector2f size{ 28,34 }, start{ 0,0 }, dSize{ 24,34 };
	Vector2f scale{ 0.714,0.588 }, sfield{ 19,22 }, tSize;
};