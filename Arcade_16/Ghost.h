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
	void findPath(Vector2i target, Vector2i curPos);
	bool move();
	//blinky
	void update(Vector2f player);
	//pinky
	void update(Vector2f player, int rot);
private:
	std::vector<std::vector<int>> field;
	int type = 0, instruction = 0;
	float speed = 0.3f;
	Animation animation;
	bool beg = 1;

	float distTraveled = 0.f;
	Vector2i prevPos;
	Vector2f pos, size{ 28,34 }, start{ 0,0 };
	Vector2f scale{ 0.714,0.588 }, sfield{ 19,22 }, tSize;
};