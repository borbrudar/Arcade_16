#include "Arkanoid.h"

Arkanoid::Arkanoid(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 450), 14);

	//setup ball
	b.loadFromFile("res/arkanoid/ball.png");
	ball.setTexture(b);
	pos.x = 230; pos.y = 380;
	ball.setPosition(pos);
	ball.setScale(0.7f, 0.7f);

	//ball collision setup
	top.setSize(Vector2f(ballW, 2.f));
	bottom.setSize(Vector2f(ballW, 2.f));
	left.setSize(Vector2f(2.f, ballH - 2.f));
	right.setSize(Vector2f(2.f, ballH - 2.f));

	//setup block
	for (int x = 0; x < sizeM; x++) {
		for (int y = 0; y < sizeN; y++) {
			blocks[x][y].setFillColor(Color::Green);
			blocks[x][y].setSize(Vector2f(sizeX, sizeY));
			blocks[x][y].setPosition(x * (sizeX + spacing) + offsetX, y * (sizeY + spacing) + offsetY);
		}
	}

	//setup paddle
	paddle.setPosition(pPos);
	paddle.setFillColor(Color::Red);
	paddle.setSize(Vector2f(90, 20));
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

	window.draw(paddle);
}

void Arkanoid::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e, float delta)
{

	if (!gameOver) {
		if (pPos.x < 0) pPos.x = 0;
		if (pPos.x > scrWidth - paddle.getSize().x) pPos.x = scrWidth - paddle.getSize().x;

		prevPos = pos;
		xturn = 0; yturn = 0;
		//clicky boi
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Left ) l = 1;
				if (e.key.code == Keyboard::Right) r = 1;
			}
			if (e.type == Event::KeyReleased) {
				if (e.key.code == Keyboard::Left) l = 0;
				if (e.key.code == Keyboard::Right) r = 0;
			}
		}
		if (back.isClicked(mouse, window)) gameState = state::menu;

		//check for borders
		if (pos.y + ballH >= scrHeight) { gameOver = 1; }
		if (pos.y <= 0) { pos.y = 0; speedy = -speedy; }
		if (pos.x <= 0) { pos.x = 0; speedx = -speedx; }
		if (pos.x >= scrWidth - ballW) { pos.x = scrWidth - ballW; speedx = -speedx; }

		//update ball position
		pos.x += speedx * delta;
		pos.y += speedy * delta;

		//update collision boxes position
		top.setPosition(pos.x + 1, pos.y);
		bottom.setPosition(Vector2f(pos.x, pos.y + ballH));
		left.setPosition(pos.x, pos.y + 1);
		right.setPosition(Vector2f(pos.x + ballW, pos.y + 1));

		//check for intersection
		bool c = 0;
		for (int x = 0; x < sizeM; x++) {
			for (int y = 0; y < sizeN; y++) {
				if (collision(blocks[x][y].getPosition(), blocks[x][y].getSize())) {
					//remove the fuckign birck
					blocks[x][y].setPosition(-100, -100);
					pos = prevPos;
					c = 1;
					break;
				}
			}
			if (c) break;
		}

		ball.setPosition(pos);

		//check for paddle

		if (l == 1) pPos.x -= pspeed * delta;
		else if (r == 1) pPos.x += pspeed * delta; 
		paddle.setPosition(pPos);
		collision(paddle.getPosition(), paddle.getSize());
	}
	else {
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::R) {
					//reset
					for (int x = 0; x < sizeM; x++) {
						for (int y = 0; y < sizeN; y++) {
							blocks[x][y].setFillColor(Color::Green);
							blocks[x][y].setSize(Vector2f(sizeX, sizeY));
							blocks[x][y].setPosition(x * (sizeX + spacing) + offsetX, y * (sizeY + spacing) + offsetY);
						}
					}
					speedx = 0.6f; speedy = 0.5f;
					pos.x = 250; pos.y = 380;
					ball.setPosition(pos);
					pPos = Vector2f(250, 420);
					paddle.setPosition(pPos);
					gameOver = 0;
				}
			}
		}
		if (back.isClicked(mouse, window)) gameState = state::menu;
	}
}

bool Arkanoid::collision(Vector2f pos, Vector2f size)
{
	int x1 = (int)top.getPosition().x, y1 = (int)top.getPosition().y;
	Vector2f temps = top.getSize();

	bool spedx = 0, spedy = 0, ret = 0;

	//top
	if (x1 < pos.x + size.x && x1 + temps.x > pos.x &&
		y1 < pos.y + size.y && y1 + temps.y > pos.y) spedy = 1;

	//bottom
	x1 = (int)bottom.getPosition().x, y1 = (int)bottom.getPosition().y;
	temps = bottom.getSize();
	if (x1 < pos.x + size.x && x1 + temps.x > pos.x &&
		y1 < pos.y + size.y && y1 + temps.y > pos.y) spedy = 1;

	//left
	x1 = (int)left.getPosition().x, y1 = (int)left.getPosition().y;
	temps = left.getSize();
	if (x1 < pos.x + size.x && x1 + temps.x > pos.x &&
		y1 < pos.y + size.y && y1 + temps.y > pos.y) spedx = 1;

	//right
	x1 = (int)right.getPosition().x, y1 = (int)right.getPosition().y;
	temps = right.getSize();
	if (x1 < pos.x + size.x && x1 + temps.x > pos.x &&
		y1 < pos.y + size.y && y1 + temps.y > pos.y) spedx = 1;

	if (spedy && !yturn) {speedy = -speedy; yturn = 1;}
	if (spedx && !xturn) {speedx = -speedx; xturn = 1;}

	if (spedy || spedx) return 1;
	return 0;
}
