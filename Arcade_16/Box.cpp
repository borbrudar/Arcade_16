#include "Box.h"

Box::Box(Vector2f pos, Vector2f size, Texture& t, Vector2f bSize, block_type type, int entity):
	entity(entity),
	type(type),
	hadEntity(entity)
{
	Vector2f bStart;
	box.setMaxSwap(0);

	switch (type) {
	case block_type::ground:
		bStart = Vector2f(0, 0);
		box.setup(t, bSize, size, 0, bStart);
		break;
	case block_type::brick:
		bStart = Vector2f(bSize.x, 0);
		box.setup(t, bSize, size, 0, bStart);
		canWiggle = 1;
		break;
	case block_type::mystery:
		bStart = Vector2f(bSize.x * 2, 0);
		box.setup(t, bSize, size, 2, bStart);
		canWiggle = 1;
		break;
	case block_type::shine:
		bStart = Vector2f(bSize.x * 6, 0);
		box.setup(t, bSize, size, 0, bStart);
		break;
	}

	box.animation.setPosition(pos);
	box.animation.setTextureRect(IntRect(bStart.x, bStart.y, bSize.x, bSize.y));

}

void Box::draw(RenderWindow& window)
{
	box.draw(window);
}

void Box::update(bool wiggle)
{
	if (wiggle) wiggling = 1;

	if (wiggling != 0 && canWiggle) {
		//move
		if (wiggling == 1) box.animation.move(0, -speed);
		else  box.animation.move(0, speed);

		if (wiggling == 1 && (oldPos.y - box.animation.getPosition().y) > (box.animation.getSize().y / 3))
			wiggling = 2;

		if (oldPos.y < box.animation.getPosition().y) {
			box.animation.setPosition(oldPos);
			wiggling = 0;
			if (hadEntity && !entity) {
				box.setMaxSwap(0);
				box.setSwap(3);
				box.setCycle(0);
				canWiggle = 0;
			}
		}
	}
	else {
		oldPos = box.animation.getPosition();
	}

	
}
