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
		box.delay = 0.25f;
		break;
	case block_type::shine:
		bStart = Vector2f(bSize.x * 6, 0);
		box.setup(t, bSize, size, 0, bStart);
		break;
	case block_type::coin:
		bStart = Vector2f(0, bSize.y);
		box.setup(t, bSize, size, 2, bStart);
		box.delay = 0.25f;
		break;
	//pipes
	case block_type::ptl:
		bStart = Vector2f(bSize.x * 3, bSize.y);
		box.setup(t, bSize, size, 0, bStart);
		break;
	case block_type::ptr:
		bStart = Vector2f(bSize.x * 4, bSize.y);
		box.setup(t, bSize, size, 0, bStart);
		break;
	case block_type::pl:
		bStart = Vector2f(bSize.x * 5, bSize.y);
		box.setup(t, bSize, size, 0, bStart);
		break;
	case block_type::pr:
		bStart = Vector2f(bSize.x * 6, bSize.y);
		box.setup(t, bSize, size, 0, bStart);
		break;
		//visual in a separate file
	case block_type::cloud1:
		box.setup(t, bSize, size, 0);
		break;
	case block_type::grass1:
		box.setup(t, bSize, size, 0);
		box.setCycle(0);
		box.setSwap(1);
		break;
	case block_type::cloud2:
		box.setRow(1);
		box.setup(t, bSize, size, 0);
		break;
	case block_type::hill1:
		box.setRow(1);
		box.setup(t, bSize, size, 0);
		box.setCycle(0);
		box.setSwap(1);
		break;
	case block_type::grass2:
		box.setRow(1);
		box.setup(t, bSize, size, 0);
		box.setCycle(0);
		box.setSwap(2);
		break;
	case block_type::cloud3:
		box.setRow(2);
		box.setup(t, bSize, size, 0);
		box.setCycle(0);
		break;
	case block_type::grass3:
		box.setRow(2);
		box.setup(t, bSize, size, 0);
		box.setCycle(0);
		box.setSwap(1);
		break;
	case block_type::hill2:
		box.setup(t, bSize, size, 0);
		break;
	}

	box.animation.setPosition(pos);
	box.animation.setTextureRect(IntRect(bStart.x, bStart.y, bSize.x, bSize.y));
	oldPos = box.animation.getPosition();
	this->pos = pos;
}

void Box::draw(RenderWindow& window)
{
	box.draw(window);
}

void Box::update(bool wiggle)
{
	box.animation.setPosition(pos.x + offX, pos.y);

	if (wiggle) wiggling = 1;

	if (wiggling != 0 && canWiggle) {
		//move
		if (wiggling == 1) pos.y -= speed;
		else  pos.y += speed;

		if (wiggling == 1 && (oldPos.y - pos.y) > (box.animation.getSize().y / 3))
			wiggling = 2;

		if (oldPos.y < pos.y) {
			pos.y = oldPos.y;
			wiggling = 0;
			//cant wiggle if its a mystery box
			if (hadEntity && !entity) {
				box.setMaxSwap(0);
				box.setSwap(3);
				box.setCycle(0);
				canWiggle = 0;
			}
		}
	}
}

void Box::off(float offX)
{
	this->offX = offX;
}
