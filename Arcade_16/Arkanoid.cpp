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

	ball.setPosition(pos);
}
