#include "Arkanoid.h"

Arkanoid::Arkanoid(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14);

	//setup ball
	b.loadFromFile("res/arkanoid/ball.png");
	ball.setTexture(b);
	pos.x = 200; pos.y = 400;
	ball.setPosition(pos);
	ball.setScale(0.7, 0.7);

	//ball collision setup
	top.setSize(Vector2f(ballW, 2));
	bottom.setSize(Vector2f(ballW, 2));
	left.setSize(Vector2f(2, ballH - 2));
	right.setSize(Vector2f(2, ballH - 2));

	//setup block
	for (int x = 0; x < sizeM; x++) {
		for (int y = 0; y < sizeN; y++) {
			blocks[x][y].setFillColor(Color::Green);
			blocks[x][y].setSize(Vector2f(sizeX, sizeY));
			blocks[x][y].setPosition(x * sizeX + offsetX + x * spacing, y * sizeY + offsetY + y * spacing);
		}
	}
}

void Arkanoid::draw(RenderWindow& window)
{
	//ball
	window.draw(ball);

	//field of bricks
	for (int x = 0; x < sizeM; x++) {
		for (int y = 0; y < sizeN; y++) {
			window.draw(blocks[x][y]);
		}
	}

	//back button
	back.draw(window);
}

void Arkanoid::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	//clicky boi
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
	}
	if (back.isClicked(mouse, window)) gameState = state::menu;

	//update ball position
	pos.x += speedx;
	pos.y += speedy;
	//check for borders
	if (pos.y >= 480) { pos.y = 480; speedy = -speedy; }
	if (pos.y <= 0) { pos.y = 0; speedy = -speedy; }
	if (pos.x <= 0) { pos.x = 0; speedx = -speedx; }
	if (pos.x >= 480) { pos.x = 480; speedx = -speedx; }

	//update collision boxes position
	top.setPosition(pos);
	bottom.setPosition(Vector2f(pos.x, pos.y + ballH));
	left.setPosition(pos.x, pos.y + 1);
	right.setPosition(Vector2f(pos.x + ballW, pos.y + 1));
	//check for intersection
	for (int x = 0; x < sizeM; x++) {
		for (int y = 0; y < sizeN; y++) {
			if (collision(blocks[x][y].getPosition(), blocks[x][y].getSize())) break;
		}
	}
	ball.setPosition(pos);
}

bool Arkanoid::collision(Vector2f pos, Vector2f size)
{
	int x1 = top.getPosition().x, y1 = top.getPosition().y;

	bool spedx = 0, spedy = 0, ret = 0;

	//top
	if (x1 < pos.x + size.y && x1 + ballW > size.x &&
		y1 < pos.y + size.y && y1 + ballH > pos.y) spedy = 1;

	//bottom
	x1 = bottom.getPosition().x, y1 = bottom.getPosition().y;
	if (x1 < pos.x + size.y && x1 + ballW > size.x &&
		y1 < pos.y + size.y && y1 + ballH > pos.y) spedy = 1;

	//left
	x1 = left.getPosition().x, y1 = left.getPosition().y;
	if (x1 < pos.x + size.y && x1 + ballW > size.x &&
		y1 < pos.y + size.y && y1 + ballH > pos.y) spedx = 1;

	//right
	x1 = right.getPosition().x, y1 = right.getPosition().y;
	if (x1 < pos.x + size.y && x1 + ballW > size.x &&
		y1 < pos.y + size.y && y1 + ballH > pos.y) spedx = 1;

	if (spedx) { speedx = -speedx; ret = 1;}
	if (spedy) { speedy = -speedy; ret = 1;}

	return ret;
}
