#include "Enemy.h"

Enemy::Enemy(Vector2f pos, Vector2f size, Vector2f tSize, Texture &t1, int type)
{
	setup(pos, size, tSize, t1, type);
}

void Enemy::setup(Vector2f pos, Vector2f size, Vector2f tSize, Texture &t1, int type)
{
	this->pos = pos;
	this->type = type;

	//animation setup
	if(type == 0) anim.setup(t1, size, tSize);
	else if(type == 1) anim.setup(t1, size, Vector2f(tSize.x ,tSize.y * 1.5f));
}

void Enemy::draw(RenderWindow& window)
{
	anim.draw(window);
}

bool Enemy::update(bool col, bool onScr)
{
	if(type == 0) anim.animation.setPosition(pos.x + offX, pos.y);
	else if(type == 1) anim.animation.setPosition(pos.x + offX, pos.y - (anim.animation.getSize().y * 0.33333f));

	onScreen = onScr;
	prevPos = pos;
	
	if (col) {
		groundTouch = 1;
		pos = prevPos;
	}
	//fall
	if (!groundTouch) pos.y += gravity;


	if (alive) {
		if (onScreen) pos.x -= speed;
		dclock.restart();
	}
	else {
		anim.setCycle(0);
		anim.setSwap(2);

		dtime = dclock.getElapsedTime().asSeconds();
		dtimer += dtime;
		dclock.restart();

		if (dtimer > ddelay) return 1;
	}

	return 0;
}

void Enemy::off(float offX)
{
	this->offX = offX;
}
