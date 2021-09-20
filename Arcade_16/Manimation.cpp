#include "Manimation.h"

void Manimation::setup(Texture& t1, Vector2f size, Vector2f tSize, int maxSwap, Vector2f start)
{
	this->size = size;
	this->start = start;
	this->maxSwap = maxSwap;
	animation.setTexture(&t1);
	animation.setTextureRect(IntRect(start.x, start.y, size.x, size.y));
	swap = 0;

	//scale
	animation.setSize(Vector2f(tSize.x, tSize.y));
}

void Manimation::draw(RenderWindow& window)
{
	time = clock.getElapsedTime().asSeconds();
	timer += time;
	clock.restart();

	if (timer > delay) {
		if (swap > maxSwap && cycle) swap = 0;

		animation.setTextureRect(IntRect(swap * size.x + start.x, row * size.y + start.y, size.x, size.y));
		
		if(cycle) swap += 1;

		timer = 0;
	}

	window.draw(animation);
}

const void Manimation::setMaxSwap(int set)
{
	maxSwap = set;
}

const void Manimation::setRow(int row)
{
	this->row = row;
}

const void Manimation::setSwap(int s)
{
	swap = s;
}

const void Manimation::setCycle(bool c)
{
	cycle = c;
}

const int Manimation::getSwap() const
{
	return swap;
}

const void Manimation::setStart(int x, int y)
{
	start = Vector2f(size.x * x, size.y * y);
}

