#include "Entity.h"

Entity::Entity(Texture& t, Vector2f size, int maxSwap, Vector2f pos, Vector2f tSize, int type):
	type(type),
	pos(pos), 
	orgPos(pos)
{
	anim.setup(t, size, Vector2f(0, 0), maxSwap);
	anim.animation.setPosition(pos);
	anim.delay = 0.1f;
	//scale
	anim.animation.setScale(tSize.x / size.x, tSize.y / size.y);
}

void Entity::draw(RenderWindow& window)
{
	anim.draw(window);
}

void Entity::update(Vector2f until)
{
	if (std::abs(pos.y - orgPos.y) < until.y) pos.y -= 1;
	anim.animation.setPosition(pos);
}

