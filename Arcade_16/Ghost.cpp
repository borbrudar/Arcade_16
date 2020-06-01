#include "Ghost.h"

void Ghost::setup(int type)
{
	this->type = type;
	up.setup("res/pacman/gh.png", size);
	up.animation.setScale(scale);
}

void Ghost::draw(RenderWindow& window)
{
	up.draw(window);
}

void Ghost::update()
{
}
