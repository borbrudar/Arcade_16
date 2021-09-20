#include "Animation.h"

Animation::Animation(std::string res1,Vector2f size)
{
	setup(res1, size);
}

void Animation::setup(std::string res1, Vector2f size, Vector2f start)
{
	this->size = size;
	this->start = start;
	t1.loadFromFile(res1);
	animation.setTexture(t1);
	animation.setTextureRect(IntRect(start.x, start.y, size.x, size.y));
	swap = 0;
}


void Animation::draw(RenderWindow& window)
{
	time = clock.getElapsedTime().asSeconds();
	timer += time;
	clock.restart();

	if (timer > delay) {
		if (swap == 1) {
			animation.setTextureRect(IntRect(swap * size.x, start.y, size.x, size.y)); swap = 0;
		}
		else if (swap == 0) {
			animation.setTextureRect(IntRect(swap * size.x , start.y, size.x, size.y)); swap = 1;
		}
		timer = 0;
	}

	window.draw(animation);
}
