#include "MP.h"

void MP::setup(Vector2f pos, Texture& t1, Vector2f pSize, Vector2f tSize)
{
	this->pos = pos;
	box.setup(t1, pSize, tSize);
	box.animation.setPosition(pos);

	//projbox
	projbox.push_back(RectangleShape(Vector2f(box.animation.getSize().x - 2, 1)));
	projbox.push_back(RectangleShape(Vector2f(box.animation.getSize().x - 2, 1)));
	projbox.push_back(RectangleShape(Vector2f(1, box.animation.getSize().y - 2)));
	projbox.push_back(RectangleShape(Vector2f(1, box.animation.getSize().y - 2)));
}

void MP::draw(RenderWindow& window)
{
	box.draw(window);
}

bool MP::update(std::vector<int> ptype)
{
	//border
	if (pos.x < 0 || pos.x > scrWidth || pos.y < 0 || pos.y > scrHeight) return 1;
	//other 
	if (ptype[1] == 1) { pos.y = prevPos.y; speedy = -speedy; box.setRow(1); }
	if (ptype[2] == 1 || ptype[3] == 1) return 1;

	pos.y += speedy;
	pos.x += speedx;

	//set jumping mechanism
	if (speedy < 0) {
		time = clock.getElapsedTime().asSeconds();
		timer += time;
		clock.restart();

		if (timer > delay) {
			box.setRow(0);
			speedy = -speedy;
			timer = 0;
		}
	}
	else clock.restart();

	prevPos = pos;
	box.animation.setPosition(pos.x + offX + oddX, pos.y);
	boxUpdate();

	return 0;
}

void MP::boxUpdate()
{
	//proj box update
	projbox[0].setPosition(box.animation.getPosition().x + 1, box.animation.getPosition().y);
	projbox[1].setPosition(box.animation.getPosition().x + 1, box.animation.getPosition().y + box.animation.getSize().y);
	projbox[2].setPosition(box.animation.getPosition().x, box.animation.getPosition().y + 1);
	projbox[3].setPosition(box.animation.getPosition().x + box.animation.getSize().x, box.animation.getPosition().y + 1);

}

void MP::off(int x)
{
	offX = x;
}
