#include "Pac_Man.h"

Pac_Man::Pac_Man(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14);

	//walls
	pp.loadFromFile("res/pacman/pipe.png");
	cp.loadFromFile("res/pacman/straight.png");
	st.loadFromFile("res/pacman/par.png");
	cr.loadFromFile("res/pacman/corner.png");
	inv.loadFromFile("res/pacman/inv.png");

	//size
	float sx = pSize.x * 0.25, sy = pSize.y * 0.25;
	tSize = Vector2i(sx, sy);

	//pacman
	pacman.setup("res/pacman/pacman.png", pSize);
	pacman.animation.setOrigin(pSize.x / 2, pSize.y / 2);
	pacman.animation.setScale(pscale, pscale);
	pacman.animation.setPosition(10 * sx - sx / 2 + start.x, 17 * sy - sy / 2 + start.y);

	//level
	lvl.loadFromFile("res/pacman/lvl.png");

	field.resize(19);
	for (int x = 0; x < lvl.getSize().x; x++) {
		field[x].resize(22);
		for (int y = 0; y < lvl.getSize().y; y++) {
			int size = walls.size();
			//parrarel
			if (lvl.getPixel(x, y) == Color(255, 255, 255, 255))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), st, start));
			if (lvl.getPixel(x, y) == Color::Black)
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), st, start, 90));
			//corners
			if (lvl.getPixel(x, y) == Color(255, 255, 255, 254))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), cr, start));
			if (lvl.getPixel(x, y) == Color(255, 255, 255, 253))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), cr, start, 90));
			if (lvl.getPixel(x, y) == Color(255, 255, 255, 252))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), cr, start, 180));
			if (lvl.getPixel(x, y) == Color(255, 255, 255, 251))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), cr, start, 270));
			//pipe
			if (lvl.getPixel(x, y) == Color(255, 0, 0, 255))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), pp, start));
			if (lvl.getPixel(x, y) == Color(255, 0, 0, 253))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), pp, start, 90));
			if (lvl.getPixel(x, y) == Color(255, 1, 1, 253))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), pp, start, 180));
			if (lvl.getPixel(x, y) == Color(255, 0, 0, 252))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), pp, start, 270));
			//straight
			if (lvl.getPixel(x, y) == Color(0, 0, 255, 255))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), cp, start));
			if (lvl.getPixel(x, y) == Color(1, 1, 255, 255))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), cp, start, 180));
			
			//im the invisible man
			if (lvl.getPixel(x, y) == Color(1, 1, 1, 1))
				walls.push_back(Wall(Vector2f(x * sx, y * sy), Vector2f(sx, sy), inv, start));

			//field
			if (walls.size() > size) field[x][y] = 1; else field[x][y] = 0;

			if (field[x][y] == 0) pellets.push_back(Pellet(Vector2f(x * sx + start.x + sx/2, y * sy + start.y + sy/2)));
		}
	}

	//ghosts
	blinky.setup(0, Vector2f{ 9,8 }, Vector2f(sx, sy), start, field);
	pinky.setup(1, Vector2f{9,10 }, Vector2f(sx, sy), start, field);
	inky.setup(2, Vector2f{ 8,10 }, Vector2f(sx, sy), start, field); 
	clyde.setup(3, Vector2f{ 10,10 }, Vector2f(sx, sy), start, field);
}

void Pac_Man::draw(RenderWindow& window)
{
	window.clear(Color::Black);
	back.draw(window);

	//walls
	for (int i = 0; i < walls.size(); i++) window.draw(walls[i].wall);

	//pellets
	for (int i = 0; i < pellets.size(); i++) pellets[i].draw(window);

	//pacman
	pacman.draw(window);

	//ghosts
	blinky.draw(window);
	pinky.draw(window);
	inky.draw(window);
	clyde.draw(window);
}

void Pac_Man::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	blinky.update(pacman.animation.getPosition());
	pinky.update(pacman.animation.getPosition(), pacman.animation.getRotation(),
		blinky.animation.animation.getPosition());
	inky.update(pacman.animation.getPosition(), pacman.animation.getRotation(), blinky.pos);
	clyde.update(pacman.animation.getPosition());

	//normalize the position
	Vector2f pos = pacman.animation.getPosition();
	Vector2i tPos = Vector2i(std::round((pos.x - start.x) / tSize.x), std::round((pos.y - start.y) / tSize.y));

	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (e.type == Event::KeyPressed) {
			switch (e.key.code) {
			case Keyboard::Up: up = 1; down = 0; left = 0; right = 0; break;
			case Keyboard::Down: down = 1; up = 0; left = 0; right = 0; break;
			case Keyboard::Left: left = 1; up = 0; down = 0; right = 0; break;
			case Keyboard::Right: right = 1; up = 0; down = 0; left = 0; break;
			}
		}
	}
	if (back.isClicked(mouse, window)) gameState = state::menu;
	
	
	
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
		if (walls[i].wall.getGlobalBounds().intersects(FloatRect(pacman.animation.getPosition() - 
			Vector2f(pSize.x * pscale / 2, pSize.y * pscale / 2),
			Vector2f(pSize.x * pscale - 2,pSize.y * pscale - 2)))) {
			pacman.animation.setPosition(prevPos);
		}
	}

	//collision with ghosts
	if (pacman.animation.getGlobalBounds().intersects(
		blinky.animation.animation.getGlobalBounds())) blinky.alive = 0;
	if (pacman.animation.getGlobalBounds().intersects(
		pinky.animation.animation.getGlobalBounds())) pinky.alive = 0;
	if (pacman.animation.getGlobalBounds().intersects(
		inky.animation.animation.getGlobalBounds())) inky.alive = 0;
	if (pacman.animation.getGlobalBounds().intersects(
		clyde.animation.animation.getGlobalBounds())) clyde.alive = 0;

}
