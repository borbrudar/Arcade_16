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
		
		if (swap > maxSwap) swap = 0;
		animation.setTextureRect(IntRect(swap * size.x, row * size.y, size.x, size.y));
		swap += 1;

		timer = 0;
	}

	window.draw(animation);
}

const void Manimation::setMaxSwap(int set)
{
	swap = 0;
	maxSwap = set;
}

const void Manimation::setRow(int row)
{
	swap = 0;
	this->row = row;
}

