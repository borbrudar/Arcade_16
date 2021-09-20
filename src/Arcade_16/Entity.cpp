#include "Entity.h"

Entity::Entity(Texture& t, Vector2f size, int maxSwap, Vector2f pos, Vector2f tSize, int type, bool bigMario):
	type(type),
	pos(pos), 
	oldPos(pos),
	bigMario(bigMario)
{
	anim.setup(t, size, tSize, maxSwap);
	anim.animation.setPosition(pos);
	oldPos = anim.animation.getPosition();
	anim.delay = 0.1f;

	if (bigMario) {
		anim.setCycle(1);
		anim.setMaxSwap(3);
		anim.setStart(1, 0);
	}

	//entity box
	entitybox.push_back(RectangleShape(Vector2f(anim.animation.getSize().x - 5, 1)));
	entitybox.push_back(RectangleShape(Vector2f(anim.animation.getSize().x - 5, 1)));
	entitybox.push_back(RectangleShape(Vector2f(1, anim.animation.getSize().y - 5)));
	entitybox.push_back(RectangleShape(Vector2f(1, anim.animation.getSize().y - 5)));

	for (int i = 0; i < entitybox.size(); i++) entitybox[i].setFillColor(Color::Red);
}

void Entity::draw(RenderWindow& window)
{
	anim.draw(window);

	if (showHitbox) for (int i = 0; i < entitybox.size(); i++) window.draw(entitybox[i]);
}

void Entity::update(Vector2f until, std::vector<int> etype)
{
	//coin
	if (type == 1) {
		if (wiggling == 1) pos.y -= speed;
		else  pos.y += speed;

		if (wiggling == 1 && (oldPos.y - pos.y) > (until.y * 2.5))
			wiggling = 2;

		if (oldPos.y < pos.y) {
			wiggling = 0;
			out = 1;
		}
	}
	//mushroom
	if (type == 2) {
		if ((oldPos.y - pos.y) < until.y && !out) pos.y -= speed / 3; else out = 1;

		//other movement
		if (out && !bigMario) {
			//collision detection
			groundTouch = 0;
			if (etype[1] == 1) { groundTouch = 1; pos.y = prevPos.y; }
			if (etype[2] == 1) { speed = -speed; pos.x = prevPos.x; }
			else if (etype[3] == 1) { speed = -speed; pos.x = prevPos.x; };
			//fall
			if (!groundTouch) pos.y += gravity;

			pos.x += speed / 2;
		}
	}

	prevPos = pos;
	anim.animation.setPosition(pos.x + offX + oddX, pos.y);
}

void Entity::off(float offX)
{
	this->offX = offX;
}

void Entity::boxUpdate()
{
	entitybox[0].setPosition(anim.animation.getPosition().x + 1, anim.animation.getPosition().y);
	entitybox[1].setPosition(anim.animation.getPosition().x + 1, anim.animation.getPosition().y + anim.animation.getSize().y);
	entitybox[2].setPosition(anim.animation.getPosition().x, anim.animation.getPosition().y + 1);
	entitybox[3].setPosition(anim.animation.getPosition().x + anim.animation.getSize().x, anim.animation.getPosition().y + 1);

}

