#include "Mario.h"


void Mario::setup(Vector2f pos, std::vector<Vector2f> msize, std::vector<Texture> &t, Vector2f tSize)
{
	prevPos = pos;
	this->pos = pos;
	tex = t;
	mSize = msize;
	this->tSize = tSize;

	box.setup(t[0], msize[0], tSize,0);
	box.delay = 0.1f;
	//mariobox
	mariobox.push_back(RectangleShape(Vector2f(box.animation.getSize().x - 2, 1)));
	mariobox.push_back(RectangleShape(Vector2f(box.animation.getSize().x - 2, 1)));
	mariobox.push_back(RectangleShape(Vector2f(1, box.animation.getSize().y - 2)));
	mariobox.push_back(RectangleShape(Vector2f(1, box.animation.getSize().y - 2)));

	for (int i = 0; i < mariobox.size(); i++) mariobox[i].setFillColor(Color::Red);
}

void Mario::boxUpdate()
{
	//mario box update
	mariobox[0].setPosition(box.animation.getPosition().x + 1, box.animation.getPosition().y);
	mariobox[1].setPosition(box.animation.getPosition().x + 1, box.animation.getPosition().y + box.animation.getSize().y);
	mariobox[2].setPosition(box.animation.getPosition().x, box.animation.getPosition().y + 1);
	mariobox[3].setPosition(box.animation.getPosition().x + box.animation.getSize().x, box.animation.getPosition().y + 1);

}

void Mario::draw(RenderWindow& window)
{
	box.draw(window);

	if(showHitbox) for (int i = 0; i < mariobox.size(); i++) window.draw(mariobox[i]);
}

bool Mario::update(bool left, bool right, bool up, bool col, std::vector<int> type, bool sprint)
{
	//check if big
	if (big && shiny && !checkShiny) {
		checkShiny = 1;
		box.setup(tex[2], mSize[1], tSize, 0);
		box.animation.setSize(Vector2f(tSize.x, tSize.y * 2));
	}
	if (big && !checkBig) {
		checkBig = 1;
		checkShiny = 0;
		box.setup(tex[1], mSize[1], tSize, 0);
		box.animation.setSize(Vector2f(tSize.x, tSize.y * 2));

		mariobox[0].setSize(Vector2f(box.animation.getSize().x - 2, 1));
		mariobox[1].setSize(Vector2f(box.animation.getSize().x - 2, 1));
		mariobox[2].setSize(Vector2f(1, box.animation.getSize().y - 2));
		mariobox[3].setSize(Vector2f(1, box.animation.getSize().y - 2));
	}
	//check if smol
	if (!big && checkBig) {
		checkBig = 0;
		checkShiny = 0;
		box.setup(tex[0], mSize[0], tSize, 0);
		box.animation.setSize(Vector2f(tSize.x, tSize.y));

		mariobox[0].setSize(Vector2f(box.animation.getSize().x - 2, 1));
		mariobox[1].setSize(Vector2f(box.animation.getSize().x - 2, 1));
		mariobox[2].setSize(Vector2f(1, box.animation.getSize().y - 2));
		mariobox[3].setSize(Vector2f(1, box.animation.getSize().y - 2));
	}

	bool ret = 0;
	//physics and stuff (temp hidden)
	{
		//other stuff
		canLeft = 1, canRight = 1;
		for (int i = 0; i < type.size(); i++) {
			if (col) {
				if (type[i] == 0) {
					jumping = 0; pos.y = prevPos.y;
				}
				if (type[i] == 1) { groundTouch = 1; pos.y = prevPos.y; }
				if (type[i] == 2) { canLeft = 0; pos.x = prevPos.x; }
				if (type[i] == 3) { canRight = 0; pos.x = prevPos.x; }

				pos = prevPos;
			}
			else groundTouch = 0;
		}

		//left/right movement
		if (pos.x <= (scrWidth / 2) || left) {
			if (left && canLeft) {
				if (sprint)  pos.x -= mariosp * sprintsp; else pos.x -= mariosp;
			}
			else if (right && canRight) {
				if (sprint)  pos.x += mariosp * sprintsp; else pos.x += mariosp;
			}
		}
		else if (right && canRight) ret = 1;

		if (pos.x < 0) pos.x = 0;

		//fall
		if (!jumping && !groundTouch) 
			//update pos
			pos.y += gravity;

		//add delay to repeated jumping
		if(!jumping){
			jdtime = jdclock.getElapsedTime().asSeconds();
			jdtimer += jdtime;
			jdclock.restart();

			if (jdtimer > jddelay) {
				noDelay = 1; jdtimer = 0; jdclock.restart();
			}
			else noDelay = 0;
		}
		else {
			jdtimer = 0;
			jdclock.restart();
		}
			

		//hop hop
		if (up && groundTouch && noDelay) jumping = 1;

		if (jumping) {
			if (!up && (gtimer > minDelay)) {
				jumping = 0;
				gtimer = 0;
			}
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
	}

	//update animation
	{
		if (right && (!prevR || !prevU)) {
			box.setCycle(1);
			box.setMaxSwap(3);
			box.setRow(0);
		}
		//left
		else if (left && (!prevL || prevU)) {
			box.setCycle(1);
			box.setMaxSwap(3);
			box.setRow(1);
		}
		//rest according to last state
		else if (!left && !right && !up) {
			if (prevR || !prevU) {
				box.setCycle(0);
				box.setRow(0);
				box.setSwap(0);
			}
			else if (prevL || prevU) {
				box.setCycle(0);
				box.setRow(1);
				box.setSwap(0);
			}
		}
		else
		//jump
		if (up) {
			box.setCycle(0);
			box.setSwap(4);
			if (prevR) {
				box.setRow(0);
				prevU = 0;
			}
			else {
				box.setRow(1);
				prevU = 1;
			}
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
		else {
			prevL = 0;
			prevR = 0;
		}
	}
	return ret;
}
