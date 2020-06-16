#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Manimation {
public:
	Manimation() = default;
	void setup(Texture &t1, Vector2f size, Vector2f tSize, int maxSwap = 1, Vector2f start = Vector2f(0, 0));
	void draw(RenderWindow& window);
	const void setMaxSwap(int set);
	const void setRow(int row);
	const void setSwap(int s);
	const void setCycle(bool c);
	const int getSwap() const;
	//size multiplier
	const void setStart(int x = 0, int y = 0);

	RectangleShape animation;
	Vector2f size;
	float delay = 0.2f;
private:
	Clock clock;
	float time = 0, timer = 0;
	int swap = 0;
	int maxSwap = 1, row = 0;
	Vector2f start;
	bool cycle = 1;
};