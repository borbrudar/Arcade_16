#include "Mario.h"


void Mario::setup(Vector2f pos, Vector2f size, Texture &t, Vector2f tSize)
{
	prevPos = pos;
	this->pos = pos;

	box.setup(t, size, tSize,0);
	box.delay = 0.125f;
	//mariobox
	mariobox.push_back(RectangleShape(Vector2f(box.animation.getSize().x, 1)));
	mariobox.push_back(RectangleShape(Vector2f(box.animation.getSize().x, 1)));
	mariobox.push_back(RectangleShape(Vector2f(1, box.animation.getSize().y - 2)));
	mariobox.push_back(RectangleShape(Vector2f(1, box.animation.getSize().y - 2)));
}

void Mario::boxUpdate()
{
	//mario box update
	mariobox[0].setPosition(box.animation.getPosition());
	mariobox[1].setPosition(box.animation.getPosition().x, box.animation.getPosition().y + box.animation.getSize().y);
	mariobox[2].setPosition(box.animation.getPosition().x, box.animation.getPosition().y + 1);
	mariobox[3].setPosition(box.animation.getPosition().x + box.animation.getSize().x, box.animation.getPosition().y + 1);

}

void Mario::draw(RenderWindow& window)
{
	box.draw(window);
}

bool Mario::update(bool left, bool right, bool up, bool col, int type)
{
	bool ret = 0;
	//physics and stuff (temp hidden)
	
		//other stuff
		if (col) {
			if (type == 1) groundTouch = 1; else groundTouch = 0;
			if (type == 0) jumping = 0;
			pos = prevPos;
		}
		else groundTouch = 0;

		//left/right movement
		if (pos.x <= (scrWidth / 2) || left) {
			if (left) pos.x -= mariosp;
			else if (right) pos.x += mariosp;
		}
		else if (right) ret = 1;

		if (pos.x < 0) pos.x = 0;

		//fall
		if (!jumping && !groundTouch) pos.y += gravity;
		//hop hop
		if (up && groundTouch) jumping = 1;

		if (jumping) {
			gtime = gclock.getElapsedTime().asSeconds();
			gtimer += gtime;
			gclock.restart();
			groundTouch = 0;

			if (gtimer > gdelay) {
				jumping = 0;
				gtimer = 0;
			}
			else  pos.y -= jump;

		}
		else gclock.restart();
		prevPos = pos;
		box.animation.setPosition(pos);
	
	//update animation
	if (right && !prevR) {
		box.setMaxSwap(3);
		box.setRow(0);
	}
	//left
	else if (left && !prevL) {
		box.setMaxSwap(3);
		box.setRow(1);
	}
	//rest according to last state
	else if (!left && !right && !up) {
		if (prevR) {
			box.setMaxSwap(0);
			box.setRow(0);
		}
		else if (prevL) {
			box.setMaxSwap(0);
			box.setRow(1);
		}
	}
	
	//jump
	if (up) {
		box.setMaxSwap(4);
		box.setSwap(4);
		if (prevR) 	box.setRow(0);
		else 	box.setRow(1);
	}

	//update prevAnim
	if (right) {
		prevR = 1;
		prevL = 0;
	}
	else if (left) {
		prevL = 1;
		prevR = 0;
	}
	
	return ret;
}

const void Mario::resetJump()
{
	jumping = 0;
	gtimer = 0;
}
