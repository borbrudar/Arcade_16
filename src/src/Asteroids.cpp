#include "Asteroids.h"

Asteroids::Asteroids(Font& f)
{	
	//scores
	scr.setPosition(20, 10);
	scr.setCharacterSize(26);
	scr.setFillColor(Color::White);
	scr.setFont(f);

	//read from file
	std::string hsc;
	hs.open("res/asteroids/hs.txt");
	std::getline(hs, hsc);
	highscore = std::stoi(hsc);
	hs.close();

	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14);

	//ship
	sh.loadFromFile("res/asteroids/ship.png");
	ship.setTexture(sh);
	ship.setScale(ship_scale, ship_scale); 
	ship.setOrigin(sh.getSize().x / 2, sh.getSize().y / 2);
	ship.setPosition(scrWidth / 2, scrHeight / 2);

	bigt.resize(3); medt.resize(3); smat.resize(3);

	bigt[0].loadFromFile("res/asteroids/big_ast1.png");
	bigt[1].loadFromFile("res/asteroids/big_ast2.png");
	bigt[2].loadFromFile("res/asteroids/big_ast3.png");

	medt[0].loadFromFile("res/asteroids/med_ast1.png");
	medt[1].loadFromFile("res/asteroids/med_ast2.png");
	medt[2].loadFromFile("res/asteroids/med_ast3.png");

	smat[0].loadFromFile("res/asteroids/small_ast1.png");
	smat[1].loadFromFile("res/asteroids/small_ast2.png");
	smat[2].loadFromFile("res/asteroids/small_ast3.png");

	//asteroids
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<int> dist(0, 2);

	for(int i = 0; i < bmax; i++) big.push_back(Astro(0, bigt[dist(engine)], pos() ) );

	//collision setup
	col.setRadius(sh.getSize().x * ship_scale / 2);
	col.setOrigin(Vector2f(col.getRadius(), col.getRadius()));
}

void Asteroids::draw(RenderWindow& window)
{
	window.clear(Color::Black);
	back.draw(window);

	window.draw(ship);

	for (int i = 0; i < bullets.size(); i++) bullets[i].draw(window);

	for (int i = 0; i < big.size(); i++) big[i].draw(window);
	for (int i = 0; i < medium.size(); i++) medium[i].draw(window);
	for (int i = 0; i < small.size(); i++) small[i].draw(window);

	//draw score and highscore
	std::string text2;
	text2.assign("Score: ");
	text2 = text2 + std::to_string(score);
	scr.setString(text2);
	scr.setPosition(20, 10);
	window.draw(scr);

	text2.assign("Highscore: ");
	text2 = text2 + std::to_string(highscore);
	scr.setString(text2);
	scr.setPosition(200, 10);
	window.draw(scr);
}

void Asteroids::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e, float delta)
{
	//update highscore
	if (score > highscore){
		highscore = score;
		//write to file and close it
		hs.open("res/asteroids/hs.txt", std::ios::out | std::ios::trunc);
		hs << highscore;
		hs.close();
	}

	if (back.isClicked(mouse, window)) gameState = state::menu;

	if (!gameOver) {
		//click click
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Left) left = 1;
				if (e.key.code == Keyboard::Right) right = 1;
				if (e.key.code == Keyboard::Space) shoot = 1;
				if (e.key.code == Keyboard::Up) move = 1;
			}
			if (e.type == Event::KeyReleased) {
				if (e.key.code == Keyboard::Left) left = 0;
				if (e.key.code == Keyboard::Right) right = 0;
				if (e.key.code == Keyboard::Space) shoot = 0;
				if (e.key.code == Keyboard::Up) move = 0;
			}
		}

		//ship rot
		if (left) rot = -rots;
		else if (right)	rot = rots;
		else rot = 0;
		ship.rotate(rot);

		//ship move
		if (move) {
			vel.x += std::cos(ship.getRotation() * 3.14159 / 180) * 0.01; //convert degress to radians
			vel.y += std::sin(ship.getRotation() * 3.14159 / 180) * 0.01;
		}
		else {
			vel.x *= drag;
			vel.y *= drag;
		}

		float sped = std::sqrt(vel.x * vel.x + vel.y * vel.y);
		if (sped > maxsped) {
			vel.x *= maxsped / sped;
			vel.y *= maxsped / sped;
		}

		ship.move(vel);
		//ship border
		if (ship.getPosition().x < 0) ship.setPosition(scrWidth, ship.getPosition().y);
		if (ship.getPosition().x > scrWidth) ship.setPosition(0, ship.getPosition().y);
		if (ship.getPosition().y < 0) ship.setPosition(ship.getPosition().x, scrHeight);
		if (ship.getPosition().y > scrHeight) ship.setPosition(ship.getPosition().x, 0);

		//ship shoot
		stime = sClock.getElapsedTime().asSeconds();
		stimer += stime;
		sClock.restart();

		if (shoot && stimer > sdelay) {
			stimer = 0;

			//calulate the right direction 
			float x = std::cos(ship.getRotation() * 3.14159 / 180); //convert degress to radians
			float y = std::sin(ship.getRotation() * 3.14159 / 180);
			Vector2f pos = ship.getPosition();
			float s = sh.getSize().x * ship_scale;

			bullets.push_back(astproj(Vector2f(pos.x + x * s, pos.y + y * s),
				Vector2f(x * bulsped, y * bulsped)));
		}
		
		//update asteroids 
		for (int i = 0; i < big.size(); i++) big[i].update();
		for (int i = 0; i < medium.size(); i++) medium[i].update();
		for (int i = 0; i < small.size(); i++) small[i].update();

		//ship asteroid collison
		col.setPosition(ship.getPosition());

		shipCol(big);
		shipCol(medium);
		shipCol(small);		

		//erase bullets which are offscreen
		for (int i = 0; i < bullets.size(); i++) if (bullets[i].isOffScreen()) bullets.erase(bullets.begin() + i);

		//bullet collision
		for (int i = 0; i < bullets.size(); i++) {
			bool b = 0;
			//big asteroids
			for (int j = 0; j < big.size(); j++) {
				if (b) break;
				//circle to circle collision
				Vector2f c1 = bullets[i].projectile.getPosition();
				Vector2f c2 = big[j].ast.getPosition();
				float r1 = bullets[i].projectile.getRadius(), r2 = big[j].r;

				float x = std::abs(c1.x - c2.x), y = std::abs(c1.y - c2.y);
				float dist = std::sqrt(x * x + y * y);

				//collision detected
				if (dist < r1 + r2) {
					score += 10;
					std::random_device rd;
					std::default_random_engine engine(rd());
					std::uniform_int_distribution<int> dist(0, 2);

					medium.push_back(Astro(1, medt[dist(engine)], big[j].ast.getPosition()));
					medium.back().vel = big[j].vel; medium.back().rot = big[j].rot;

					medium.push_back(Astro(1, medt[dist(engine)], big[j].ast.getPosition()));
					medium.back().vel = Vector2f(-big[j].vel.x, -big[j].vel.y); medium.back().rot = -big[j].rot;

					bullets.erase(bullets.begin() + i);
					big.erase(big.begin() + j);
					b = 1;
					break;
				}

			}
			//medium asteroids
			for (int j = 0; j < medium.size(); j++) {
				if (b) break;
				//circle to circle collision
				Vector2f c1 = bullets[i].projectile.getPosition();
				Vector2f c2 = medium[j].ast.getPosition();
				float r1 = bullets[i].projectile.getRadius(), r2 = medium[j].r;

				float x = std::abs(c1.x - c2.x), y = std::abs(c1.y - c2.y);
				float dist = std::sqrt(x * x + y * y);

				//collision detected
				if (dist < r1 + r2) {
					score += 20;
					std::random_device rd;
					std::default_random_engine engine(rd());
					std::uniform_int_distribution<int> dist(0, 2);

					small.push_back(Astro(2, smat[dist(engine)], medium[j].ast.getPosition()));
					small.back().vel = medium[j].vel; small.back().rot = medium[j].rot;

					small.push_back(Astro(2, smat[dist(engine)], medium[j].ast.getPosition()));
					small.back().vel = Vector2f(-medium[j].vel.x, -medium[j].vel.y); medium.back().rot = -medium[j].rot;

					bullets.erase(bullets.begin() + i);
					medium.erase(medium.begin() + j);
					b = 1;
					break;
				}
			}
			//small asteroids
			for (int j = 0; j < small.size(); j++) {
				if (b) break;
				//circle to circle collision
				Vector2f c1 = bullets[i].projectile.getPosition();
				Vector2f c2 = small[j].ast.getPosition();
				float r1 = bullets[i].projectile.getRadius(), r2 = small[j].r;

				float x = std::abs(c1.x - c2.x), y = std::abs(c1.y - c2.y);
				float dist = std::sqrt(x * x + y * y);

				//collision detected
				if (dist < r1 + r2) {
					score += 30;
					bullets.erase(bullets.begin() + i);
					small.erase(small.begin() + j);
					b = 1;
					break;
				}
			}

			if (b) break;
		}

		//add new asteroids
		if (big.size() < bmax && medium.size() < mmax && small.size() < smax) {
			atime = aClock.getElapsedTime().asSeconds();
			atimer += atime;
			aClock.restart();

			if (atimer > adelay) {
				std::random_device rd;
				std::default_random_engine engine(rd());
				std::uniform_int_distribution<int> dist(0, 2);

				big.push_back(Astro(0, bigt[dist(engine)], pos()));
				atimer = 0;
			}
		}
	}
    else {
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (e.type == Event::KeyPressed) {
			if (e.key.code == Keyboard::R) {
				//reset
				gameOver = 0;
				ship.setPosition(scrWidth / 2, scrHeight / 2);

				big.clear(); medium.clear(); small.clear();
				std::random_device rd;
				std::default_random_engine engine(rd());
				std::uniform_int_distribution<int> dist(0, 2);

				for (int i = 0; i < bmax; i++) big.push_back(Astro(0, bigt[dist(engine)], pos()));

				move = 0, left = 0, right = 0, score = 0;
			}
		  }
		}
    }


}

Vector2f Asteroids::pos()
{
	float x1 = 0, y1 = 0;
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<int> xd(0, scrWidth - bigt[0].getSize().x);
	std::uniform_int_distribution<int> yd(0, scrHeight - bigt[0].getSize().x);
	std::uniform_int_distribution<int> p(0, 1);

	if (p(engine) == 0) {
		x1 = 0;
		y1 = yd(engine);
	}
	else {
		x1 = xd(engine);
		y1 = 0;
	}
	return Vector2f(x1, y1);
}

void Asteroids::shipCol(std::vector<Astro>& vec)
{
	for (int i = 0; i < vec.size(); i++) {
		Vector2f c1 = vec[i].ast.getPosition();
		Vector2f c2 = col.getPosition();
		float r1 = vec[i].ast.getRadius(), r2 = col.getRadius();

		float x = std::abs(c1.x - c2.x), y = std::abs(c1.y - c2.y);
		float dist = std::sqrt(x * x + y * y);

		//collision detected
		if (dist < r1 + r2) gameOver = 1;
	}
}

astproj::astproj(Vector2f pos, Vector2f vel) : pos(pos), vel(vel)
{
	projectile.setRadius(ast_r);
	projectile.setFillColor(Color::White);
	projectile.setPosition(pos);
}

void astproj::draw(RenderWindow& window)
{
	projectile.move(vel);
	pos = projectile.getPosition();
	window.draw(projectile);
}

bool astproj::isOffScreen()
{
	if (pos.x < 0 || pos.x + ast_r > scrWidth || 
	pos.y < 0 || pos.y + ast_r > scrHeight) return true;

	return false;
}


