#include "Mario.h"


void Mario::setup(Vector2f pos, Vector2f size, Texture &t)
{
	prevPos = pos;
	this->pos = pos;
	box.setPosition(pos);
	box.setSize(size);
	box.setTexture(&t);
}

void Mario::draw(RenderWindow& window)
{
	window.draw(box);
}

void Mario::update(bool left, bool right, bool up, bool col)
{
	if (col) {
		pos = prevPos;
		groundTouch = 1;
	}

	//left/right movement
	prevPos = pos;
	if (left) pos.x -= speed;
	else if (right) pos.x += speed;

	//fall
	if (!groundTouch) pos.y += gravity;
	//hop hop
	if (up && groundTouch) 	jumping = 1;
	if (jumping) {
		gtimer = gclock.getElapsedTime().asSeconds();
		gtime += gtimer;
		gclock.restart();

		if (gtime > gdelay) {
			groundTouch = 0;
			jumping = 0;
			gtime = 0;
		}
		else pos.y -= jump;
	}
	else gclock.restart();

	box.setPosition(pos);
}
