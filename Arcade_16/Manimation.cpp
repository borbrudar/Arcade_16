#include "Manimation.h"

Manimation::Manimation(Texture &t1, Vector2f size)
{
	setup(t1, size);
}

void Manimation::setup(Texture& t1, Vector2f size, Vector2f start, int maxSwap)
{
	this->size = size;
	this->start = start;
	this->maxSwap = maxSwap;
	animation.setTexture(t1);
	animation.setTextureRect(IntRect(start.x, start.y, size.x, size.y));
	swap = 0;
}


void Manimation::draw(RenderWindow& window)
{
	time = clock.getElapsedTime().asSeconds();
	timer += time;
	clock.restart();

	if (timer > delay) {
		
		if (swap > maxSwap) swap = 0;
		animation.setTextureRect(IntRect(swap * size.x, start.y, size.x, size.y));
		swap += 1;

		timer = 0;
	}

	window.draw(animation);
}

