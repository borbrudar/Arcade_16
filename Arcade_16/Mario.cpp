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

bool Mario::update(bool left, bool right, bool up, bool col, int type)
{
	prevPos = pos;
	if (col) {
		if (type == 1) groundTouch = 1; else groundTouch = 0;
		if (type == 0) jumping = 0;
		pos = prevPos;
	}
	else groundTouch = 0;

	//left/right movement
	bool ret = 0;
	if (pos.x <= (scrWidth / 2) || left) {
		if (left) pos.x -= mariosp;
		else if (right) pos.x += mariosp;
	}
	else if (right) ret = 1;

	if (pos.x < 0) pos.x = 0;

	//fall
	if (!jumping && !groundTouch) pos.y += gravity;
	//hop hop
	if (up && groundTouch) 	jumping = 1;

	if (jumping) {
		gtimer = gclock.getElapsedTime().asSeconds();
		gtime += gtimer;
		gclock.restart();
		groundTouch = 0;

		if (gtime > gdelay) {
			jumping = 0;
			gtime = 0;
		}
		else  pos.y -= jump;
		
	}
	else gclock.restart();

	box.setPosition(pos);
	return ret;
}
