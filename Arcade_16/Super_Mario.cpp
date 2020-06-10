#include "Super_Mario.h"

Super_Mario::Super_Mario(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14);

	//ground
	gr.loadFromFile("res/mario/ground.png");


	//mario
	mr.loadFromFile("res/mario/mario.png");

	//level
	lvl.loadFromFile("res/mario/lvl.png");
	float sy = scrHeight / lvl.getSize().y, sx = sy;

	for (int x = 0; x < lvl.getSize().x; x++) {
		for (int y = 0; y < lvl.getSize().y; y++) {
			if (lvl.getPixel(x, y) == Color(0, 0, 0, 255))
				ground.push_back(Ground(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx,sy), gr));
			if (lvl.getPixel(x, y) == Color(255, 0, 0, 255))
				mario.setup(Vector2f(x * sx + off.x, y * sy + off.y - 100), Vector2f(sx, sy), mr);
		}
	}

}

void Super_Mario::draw(RenderWindow& window)
{
	window.clear(Color(92, 148, 252));
	back.draw(window);

	for (int i = 0; i < ground.size(); i++) ground[i].draw(window);

	mario.draw(window);
}

void Super_Mario::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{

	if (back.isClicked(mouse, window)) gameState = state::menu;

	//input
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (e.type == Event::KeyPressed) {
			switch (e.key.code) {
			case Keyboard::Left: left = 1; break;
			case Keyboard::Right: right = 1; break;
			case Keyboard::Up: up = 1; break;
			}
		}
		if (e.type == Event::KeyReleased) {
			switch (e.key.code) {
			case Keyboard::Left: left = 0; break;
			case Keyboard::Right: right = 0; break;
			case Keyboard::Up: up = 0; break;
			}
		}

	}

	//ground collision
	bool col = 0;
	for (int i = 0; i < ground.size(); i++) {
		if (ground[i].box.getGlobalBounds().intersects(mario.box.getGlobalBounds())) {
			col = 1; 
			break;
		}
	}

	mario.update(left, right, up, col);
}
