#include "Entity.h"

Entity::Entity(Texture& t, Vector2f size, int maxSwap, Vector2f pos, Vector2f tSize, int type):
	type(type),
	pos(pos), 
	orgPos(pos)
{
	anim.setup(t, size, tSize, maxSwap);
	anim.animation.setPosition(pos);
	anim.delay = 0.1f;
}

void Entity::draw(RenderWindow& window)
{
	anim.draw(window);
}

void Entity::update(Vector2f until)
{
	if (std::abs(pos.y - orgPos.y) < until.y) pos.y -= 1; else out = 1;
	anim.animation.setPosition(pos);
}

