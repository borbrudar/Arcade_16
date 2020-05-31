#include "Pac_Man.h"

Pac_Man::Pac_Man(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14);

	//pacman
	pacman.setup("res/pacman/pacman.png", pSize);
	pacman.animation.setOrigin(pSize.x / 2, pSize.y / 2);
	pacman.animation.setScale(pscale, pscale);
	pacman.animation.setPosition(scrWidth / 2, scrHeight / 2);

	//walls
	pp.loadFromFile("res/pacman/pipe.png");
	cp.loadFromFile("res/pacman/straight.png");
	st.loadFromFile("res/pacman/par.png");

	//level
	float sx = pSize.x * pscale, sy = pSize.y * pscale;
	lvl.loadFromFile("res/pacman/lvl.png");

	for (int x = 0; x < lvl.getSize().x; x++) {
		for (int y = 0; y < lvl.getSize().y; y++) {
			if (lvl.getPixel(x, y) == Color::White)
				walls.push_back(Wall(Vector2f(x * sx + sx, y * sy), Vector2f(sx, sy), st));
			if (lvl.getPixel(x, y) == Color::Black)
				walls.push_back(Wall(Vector2f(x * sx + sx, y * sy), Vector2f(sx, sy), st, 90));

		}
	}
}

void Pac_Man::draw(RenderWindow& window)
{
	window.clear(Color::Black);
	back.draw(window);

	pacman.draw(window);

	for (int i = 0; i < walls.size(); i++) window.draw(walls[i].wall);
}

void Pac_Man::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (e.type == Event::KeyPressed) {
			switch (e.key.code) {
			case Keyboard::Up: up = 1; break;
			case Keyboard::Down: down = 1; break;
			case Keyboard::Left : left = 1; break;
			case Keyboard::Right: right = 1; break;
			}
		}
		if (e.type == Event::KeyReleased) {
			switch (e.key.code) {
			case Keyboard::Up: up = 0; break;
			case Keyboard::Down: down = 0; break;
			case Keyboard::Left: left = 0; break;
			case Keyboard::Right: right = 0; break;
			}
		}
	}

	//movement
	prevPos = pacman.animation.getPosition();
	if (up) {
		pacman.animation.move(0, -speed);
		pacman.animation.setRotation(270);
	}
	else if (down) {
		pacman.animation.move(0, speed);
		pacman.animation.setRotation(90);
	}
	else if (left) {
		pacman.animation.move(-speed, 0);
		pacman.animation.setRotation(180);
	}
	else if (right) {
		pacman.animation.move(speed, 0);
		pacman.animation.setRotation(0);
	}

	//collision
	for (int i = 0; i < walls.size(); i++) {
		if (walls[i].wall.getGlobalBounds().intersects(pacman.animation.getGlobalBounds())) {
			pacman.animation.setPosition(prevPos);
		}
	}

	if (back.isClicked(mouse, window)) gameState = state::menu;
}
