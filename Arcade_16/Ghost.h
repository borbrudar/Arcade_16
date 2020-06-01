#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"

using namespace sf;

class Ghost {
public:
	void setup(int type);
	void draw(RenderWindow& window);
	void update();

private:
	int type = 0;
	Vector2f pos;
	Vector2f size{ 28,34 };
	Vector2f scale{ 0.714,0.588 };
	Animation up;
};