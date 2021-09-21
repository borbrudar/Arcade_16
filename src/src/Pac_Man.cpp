#include "Pac_Man.h"

Pac_Man::Pac_Man(Font& f)
{
	//scores
	scr.setPosition(20, 10);
	scr.setCharacterSize(20);
	scr.setFillColor(Color::White);
	scr.setFont(f);

	//read from file
	std::string hsc;
	hs.open("res/pacman/hs.txt");
	std::getline(hs, hsc);
	highscore = std::stoi(hsc);
	hs.close();

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

	setup();
}

void Pac_Man::setup()
{
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

			if (field[x][y] == 0) {
				//except some parts
				if (!(x == 0 && y == 8) && !(x == 1 && y == 8) && !(x == 2 && y == 8) &&
					!(x == 0 && y == 12) && !(x == 1 && y == 12) && !(x == 2 && y == 12) &&
					!(x == 16 && y == 8) && !(x == 17 && y == 8) && !(x == 18 && y == 8) &&
					!(x == 16 && y == 12) && !(x == 17 && y == 12) && !(x == 18 && y == 12) &&
					!(x == 8 && y == 10) && !(x == 9 && y == 10) && !(x == 10 && y == 10)) {

					bool super = 0;
					if ((x == 1 && y == 2) || (x == 17 && y == 2) ||
						(x == 1 && y == 19) || (x == 17 && y == 19)) super = 1;
					pellets.push_back(Pellet(Vector2f(x * sx + start.x + sx / 2, y * sy + start.y + sy / 2), super));
				}
			}
		}
	}

	//ghosts
	blinky.setup(0, Vector2f{ 9,8 }, Vector2f(sx, sy), start, field);
	pinky.setup(1, Vector2f{ 9,10 }, Vector2f(sx, sy), start, field);
	inky.setup(2, Vector2f{ 8,10 }, Vector2f(sx, sy), start, field);
	clyde.setup(3, Vector2f{ 10,10 }, Vector2f(sx, sy), start, field);

	blinky.reset(); pinky.reset(); inky.reset(); clyde.reset();
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

	//draw score and highscore
	std::string text2;
	text2.assign("Score: ");
	text2 = text2 + std::to_string(score);
	scr.setString(text2);
	scr.setPosition(20, 30);
	window.draw(scr);

	text2.assign("Highscore: ");
	text2 = text2 + std::to_string(highscore);
	scr.setString(text2);
	scr.setPosition(20, 100);
	window.draw(scr);
}

void Pac_Man::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e, float delta)
{
	//highscore update
	if (score > highscore) {
		highscore = score;
		//write to file and close it
		hs.open("res/pacman/hs.txt", std::ios::out | std::ios::trunc);
		hs << highscore;
		hs.close();
	}
	if (back.isClicked(mouse, window)) gameState = state::menu;

	if (!gameOver) {
		//update the ghosts
		blinky.update(pacman.animation.getPosition(), frightened);
		pinky.update(pacman.animation.getPosition(), pacman.animation.getRotation(),
			blinky.animation.animation.getPosition(), frightened);
		inky.update(pacman.animation.getPosition(), pacman.animation.getRotation(), blinky.pos, frightened);
		clyde.update(pacman.animation.getPosition(), frightened);

		//frightened mode update
		if (frightened) {
			ftimer = fclock.getElapsedTime().asSeconds();
			ftime += ftimer;
			fclock.restart();

			if (ftime > fdelay) frightened = 0;
		}
		else {
			fclock.restart();
			ftime = 0;
		}

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
				Vector2f(pSize.x * pscale - 2, pSize.y * pscale - 2)))) {
				pacman.animation.setPosition(prevPos);
			}
		}

		//collision with ghosts
		if (pacman.animation.getGlobalBounds().intersects(
			blinky.animation.animation.getGlobalBounds()) && frightened) blinky.alive = 0;
		if (pacman.animation.getGlobalBounds().intersects(
			pinky.animation.animation.getGlobalBounds()) && frightened) pinky.alive = 0;
		if (pacman.animation.getGlobalBounds().intersects(
			inky.animation.animation.getGlobalBounds()) && frightened) inky.alive = 0;
		if (pacman.animation.getGlobalBounds().intersects(
			clyde.animation.animation.getGlobalBounds()) && frightened) clyde.alive = 0;

		//collision with ghosts
		if (pacman.animation.getGlobalBounds().intersects(
			blinky.animation.animation.getGlobalBounds()) && blinky.alive && !frightened) gameOver = 1;
		if (pacman.animation.getGlobalBounds().intersects(
			pinky.animation.animation.getGlobalBounds()) && pinky.alive && !frightened) gameOver = 1;
		if (pacman.animation.getGlobalBounds().intersects(
			inky.animation.animation.getGlobalBounds()) && inky.alive && !frightened) gameOver = 1;
		if (pacman.animation.getGlobalBounds().intersects(
			clyde.animation.animation.getGlobalBounds()) && clyde.alive && !frightened) gameOver = 1;


		//collision with pellets
		for (int i = 0; i < pellets.size(); i++) {
			if (pacman.animation.getGlobalBounds().intersects(pellets[i].pellet.getGlobalBounds())) {
				if (pellets[i].super == 1) {
					frightened = 1;
					ftime = 0;
					score += 40;
				}
				score += 10;
				pellets.erase(pellets.begin() + i);
			}
		}

	}
	else {
		while (window.pollEvent(e)) {
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::R) {
					//game reset
					gameOver = 0;
					score = 0;
					setup();
				}
			}
		}
	}
}
