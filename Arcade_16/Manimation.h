#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Manimation {
public:
	Manimation() = default;
	Manimation(Texture& t1, Vector2f size);
	void setup(Texture &t1, Vector2f size, Vector2f start = Vector2f(0, 0));
	void draw(RenderWindow& window);

	Sprite animation;
	Vector2f size;
	float delay = 0.2f;
private:
	Clock clock;
	float time = 0, timer = 0;
	int swap = 0;
	Vector2f start;
};