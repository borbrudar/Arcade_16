#include "Enemy.h"

Enemy::Enemy(Vector2f pos, Vector2f size, Vector2f tSize, Texture &t1)
{
	setup(pos, size, tSize, t1);
}

void Enemy::setup(Vector2f pos, Vector2f size, Vector2f tSize, Texture &t1)
{
	this->pos = pos;
	anim.setup(t1, size);

	death.setTexture(&t1);
	death.setSize(tSize);
	death.setTextureRect(IntRect(size.x * 2, 0, size.x, size.y));

	//scale
	anim.animation.setScale(tSize.x / size.x, tSize.y / size.y);
}

void Enemy::draw(RenderWindow& window)
{
	if (alive) anim.draw(window);
	else window.draw(death);
}

bool Enemy::update(bool col)
{
	prevPos = pos;
	
	if (col) {
		groundTouch = 1;
		pos = prevPos;
	}
	//fall
	if (!groundTouch) pos.y += gravity;


	if (alive) {
		if (onScreen) pos.x -= speed;
		anim.animation.setPosition(pos);

		dclock.restart();
	}
	else {
		death.setPosition(pos);

		dtime = dclock.getElapsedTime().asSeconds();
		dtimer += dtime;
		dclock.restart();

		if (dtimer > ddelay) return 1;
	}

	return 0;
}
