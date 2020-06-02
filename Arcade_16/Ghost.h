#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <random>

using namespace sf;

class Ghost {
public:
	void setup(int type, Vector2f pos, Vector2f size, Vector2f start, std::vector<std::vector<int>> field);
	void draw(RenderWindow& window);
	void update(Vector2f player);
	void findPath(Vector2f target, Vector2f curPos);

private:
	std::vector<std::vector<int>> field;
	int type = 0; 
	Vector2f pos, size{ 28,34 }, start{ 0,0 }, scale{ 0.714,0.588 }, sfield{ 19,22 }, tSize;
	Animation up;
	std::vector<RectangleShape> test;
	std::vector<Vector2f> path;
	bool foundPath = 0, lr, ud;
};