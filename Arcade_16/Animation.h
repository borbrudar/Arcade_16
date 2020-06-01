#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Animation {
public:
	Animation() = default;
	Animation(std::string res1, Vector2f size);
	void setup(std::string res1, Vector2f size, Vector2f start = Vector2f(0,0));
	void draw(RenderWindow& window);

	Sprite animation;
	Vector2f size;

private:
	Clock clock;
	float time = 0, timer = 0, delay = 0.2f;
	Texture t1;
	int swap = 0;
};