#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Animation {
public:
	Animation(std::string res1, std::string res2);
	void draw(RenderWindow& window);

	Clock clock;
	float time = 0, timer = 0, delay = 0.2f;
	Texture t1, t2;
	Sprite animation;
	int swap = 0;
};