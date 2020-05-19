#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Animation {
public:
	Animation() = default;
	Animation(std::string res1, Vector2f size);
	void setup(std::string res1, Vector2f size);
	void draw(RenderWindow& window);

	Sprite animation;

private:
	Clock clock;
	float time = 0, timer = 0, delay = 0.2f;
	Texture t1, t2;
	int swap = 0;
	Vector2f size;
};