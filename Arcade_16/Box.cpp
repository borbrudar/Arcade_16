#include "Box.h"

Box::Box(Vector2f pos, Vector2f size, Texture& t)
{
	box.setPosition(pos);
	box.setTexture(&t);
	box.setSize(size);
}

void Box::draw(RenderWindow& window)
{
	window.draw(box);
}
