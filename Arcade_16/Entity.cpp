#include "Entity.h"

Entity::Entity(Texture& t, Vector2f size, int maxSwap, Vector2f pos, Vector2f tSize, int type):
	type(type),
	pos(pos), 
	oldPos(pos)
{
	anim.setup(t, size, tSize, maxSwap);
	anim.animation.setPosition(pos);
	oldPos = anim.animation.getPosition();
	anim.delay = 0.1f;
}

void Entity::draw(RenderWindow& window)
{
	anim.draw(window);
}

void Entity::update(Vector2f until)
{
	anim.animation.setPosition(pos.x + offX + oddX, pos.y);
	//coin
	if (type == 1) {
		if (wiggling == 1) pos.y -= speed;
		else  pos.y += speed;

		if (wiggling == 1 && (oldPos.y - pos.y) > (until.y * 2.5))
			wiggling = 2;

		if (oldPos.y < pos.y) {
			wiggling = 0;
			out = 1;
		}
	}
	if (type == 2) {
		if((oldPos.y - pos.y) < until.y) pos.y -= speed;
	}

	
}

void Entity::off(float offX)
{
	this->offX = offX;
}

