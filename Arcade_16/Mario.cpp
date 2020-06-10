#include "Mario.h"


void Mario::setup(Vector2f pos, Vector2f size, Texture &t)
{
	this->pos = pos;
	box.setPosition(pos);
	box.setSize(size);
	box.setTexture(&t);
}

void Mario::draw(RenderWindow& window)
{
	window.draw(box);
}

void Mario::update(bool left, bool right)
{
	if (left) pos.x -= speed;
	else if (right) pos.x += speed;

	box.setPosition(pos);
}
