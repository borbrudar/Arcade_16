#include "Pong.h"

Pong::Pong(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::White, Vector2f(40, 30), Vector2f(scrWidth / 2 - 20, 10), 14);

	//player setup
	player.setFillColor(Color::White);
	player.setSize(Vector2f(10, 40));
	player.setPosition(50, scrHeight / 2);

	//ball
	ball.setFillColor(Color::White);
	ball.setSize(Vector2f(10, 10));
	ball.setPosition(scrWidth / 2, 0);

	//line
	line.setFillColor(Color::White);
	line.setSize(Vector2f(5, 20));
}

void Pong::draw(RenderWindow& window)
{
	window.clear(Color::Black);

	//draw line
	for (int i = 0; i < scrHeight / line.getSize().y / 2; i++) {
		line.setPosition(scrWidth / 2, i * (line.getSize().y * 2) + line.getSize().y / 2);
		window.draw(line);
	}

	back.draw(window);
	window.draw(player);
	window.draw(ball);
}

void Pong::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	//clicky boi
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (e.type == Event::KeyPressed) {
			if (e.key.code == Keyboard::Down) down = 1;
			if (e.key.code == Keyboard::Up) up = 1;
		}
		if (e.type == Event::KeyReleased) {
			if (e.key.code == Keyboard::Down) down = 0;
			if (e.key.code == Keyboard::Up) up = 0;
		}
	}
	if (back.isClicked(mouse, window)) gameState = state::menu;

	if (up) speedy = -csped;
	else if (down) speedy = csped;
	else speedy = 0;

	//border stuff
	if (player.getPosition().y < 0) player.setPosition(player.getPosition().x, 0);
	if (player.getPosition().y + player.getSize().y > scrHeight)
	player.setPosition(player.getPosition().x, scrHeight - player.getSize().y);
	
	player.move(0, speedy);

	//ball stuff
	if (ball.getGlobalBounds().intersects(player.getGlobalBounds())) bspedx = -bspedx;
	//ball border
	if (ball.getPosition().y < 0) bspedy = -bspedy;
	if (ball.getPosition().y + ball.getSize().y > scrHeight) bspedy = -bspedy;
	//ball reset
	if (ball.getPosition().x < 0 || ball.getPosition().x + ball.getSize().x > scrWidth) {
		ball.setPosition(scrWidth / 2, 0);
		bspedx = orgx; bspedy = orgy;
	}

	ball.move(bspedx, bspedy);
}
