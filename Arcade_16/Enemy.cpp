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
	anim.setup(t1, size, tSize);

	//enemybox
	enemybox.push_back(RectangleShape(Vector2f(anim.animation.getSize().x - 5, 1)));
	enemybox.push_back(RectangleShape(Vector2f(anim.animation.getSize().x - 5, 1)));
	enemybox.push_back(RectangleShape(Vector2f(1, anim.animation.getSize().y - 5)));
	enemybox.push_back(RectangleShape(Vector2f(1, anim.animation.getSize().y - 5)));

	for (int i = 0; i < enemybox.size(); i++) enemybox[i].setFillColor(Color::Red);
}

void Enemy::boxUpdate()
{
	//mario box update
	enemybox[0].setPosition(anim.animation.getPosition().x + 1, anim.animation.getPosition().y);
	enemybox[1].setPosition(anim.animation.getPosition().x + 1, anim.animation.getPosition().y + anim.animation.getSize().y);
	enemybox[2].setPosition(anim.animation.getPosition().x, anim.animation.getPosition().y + 1);
	enemybox[3].setPosition(anim.animation.getPosition().x + anim.animation.getSize().x, anim.animation.getPosition().y + 1);

}

void Enemy::draw(RenderWindow& window)
{
	anim.draw(window);

	if(showHitbox) for (int i = 0; i < enemybox.size(); i++) window.draw(enemybox[i]);
}

bool Enemy::update(std::vector<int> etype, bool onScr)
{
	onScreen = onScr;

	//collision detection
	groundTouch = 0;
	if (etype[1] == 1) { groundTouch = 1; pos.y = prevPos.y; }
	if (etype[2] == 1) { speed = -speed; pos.x = prevPos.x; }
	else if (etype[3] == 1) { speed = -speed; pos.x = prevPos.x; };
	//fall
	if (!groundTouch) pos.y += gravity;

	//hybrid
	if (alive) {
		if (onScreen) pos.x -= speed;
		dclock.restart();
	}
	else if (type == 0) {
		dtime = dclock.getElapsedTime().asSeconds();
		dtimer += dtime;
		dclock.restart();

		if (dtimer > ddelay) return 1;
	}

	//animation setup
	else if (type == 1 && !deathSet) {
		anim.animation.setSize(Vector2f(anim.animation.getSize().x, anim.animation.getSize().y / 3 * 2));

		enemybox[0].setSize(Vector2f(anim.animation.getSize().x, 1));
		enemybox[1].setSize(Vector2f(anim.animation.getSize().x, 1));
		enemybox[2].setSize(Vector2f(1, anim.animation.getSize().y - 5));
		enemybox[3].setSize(Vector2f(1, anim.animation.getSize().y - 5));
		deathSet = 1;
	}
	if (!alive && spin == 0) {
		anim.setCycle(0);
		anim.setSwap(2);
	}

	if (spinning) {
		anim.setCycle(1);
		anim.setSwap(0);
		anim.setStart(3, 0);
		anim.setMaxSwap(3);
		anim.delay = 0.1f;
		spinning = 0;
	}

	//spinny boi
	if (spin != 0) {
		if (spin == 1) pos.x += 4 * speed; else pos.x -= 4 * speed;
		
	}

	
	if (type == 1 && alive) anim.animation.setPosition(pos.x + offX, pos.y - (anim.animation.getSize().y / 3));
	else anim.animation.setPosition(pos.x + offX, pos.y);
	prevPos = pos;

	return 0;
}

void Enemy::off(float offX)
{
	this->offX = offX;
}
