#include "Box.h"

Box::Box(Vector2f pos, Vector2f size, Texture& t, Vector2f bSize, Vector2f bStart)
{
	box.setPosition(pos);
	box.setTexture(&t);
	box.setSize(size);

	box.setTextureRect(IntRect(bStart.x, bStart.y, bSize.x, bSize.y));
}

void Box::draw(RenderWindow& window)
{
	window.draw(box);
}
