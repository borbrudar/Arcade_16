#include "Animation.h"

Animation::Animation(std::string res1, std::string res2)
{
	t1.loadFromFile(res1);
	t2.loadFromFile(res2);
	animation.setTexture(t1);
	swap = 0;
}

void Animation::draw(RenderWindow& window)
{
	time = clock.getElapsedTime().asSeconds();
	timer += time;
	clock.restart();
	delay = 0.2f;

	if (timer > delay) {
		if (swap == 1) {
			animation.setTexture(t1); swap = 0;
		}
		else if (swap == 0) {
			animation.setTexture(t2); swap = 1;
		}
		timer = 0;
	}

	window.draw(animation);
}
