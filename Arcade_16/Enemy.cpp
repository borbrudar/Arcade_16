#include "Enemy.h"

Enemy::Enemy(Vector2f pos, Vector2f size, Vector2f tSize, Texture &t1)
{
	setup(pos, size, tSize, t1);
}

void Enemy::setup(Vector2f pos, Vector2f size, Vector2f tSize, Texture &t1)
{
	this->pos = pos;
	anim.setup(t1, size);

	//scale
	anim.animation.setScale(tSize.x / size.x, tSize.y / size.y);
}

void Enemy::draw(RenderWindow& window)
{
	anim.draw(window);
}

void Enemy::update(bool col)
{
	prevPos = pos;
	if (col) {
		groundTouch = 1;
		pos = prevPos;
	}

	if (onScreen) pos.x -= speed;

	//fall
	if (!groundTouch) pos.y += gravity;

	anim.animation.setPosition(pos);
}
