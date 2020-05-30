#include "Asteroids.h"

Asteroids::Asteroids(Font& f)
{	
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

	big.push_back(Astro(0, bigt[0], Vector2f(100, 100)));

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

}

void Asteroids::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
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
	if (back.isClicked(mouse, window)) gameState = state::menu;

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

	//ship asteroid collison
	col.setPosition(ship.getPosition());
	//big
	for (int i = 0; i < big.size(); i++) {
		Vector2f c1 = big[i].ast.getPosition();
		Vector2f c2 = col.getPosition();
		float r1 = big[i].ast.getRadius(), r2 = col.getRadius();

		float x = std::abs(c1.x - c2.x), y = std::abs(c1.y - c2.y);
		float dist = std::sqrt(x * x + y * y);

		//collision detected
		if (dist < r1 + r2) {
			std::cout << "COL" << std::endl;
		}
	}
	//medium
	for (int i = 0; i < medium.size(); i++) {
		Vector2f c1 = medium[i].ast.getPosition();
		Vector2f c2 = col.getPosition();
		float r1 = medium[i].ast.getRadius(), r2 = col.getRadius();

		float x = std::abs(c1.x - c2.x), y = std::abs(c1.y - c2.y);
		float dist = std::sqrt(x * x + y * y);

		//collision detected
		if (dist < r1 + r2) {
			std::cout << "COL" << std::endl;
		}
	}
	//small
	for (int i = 0; i < small.size(); i++) {
		Vector2f c1 = small[i].ast.getPosition();
		Vector2f c2 = col.getPosition();
		float r1 = small[i].ast.getRadius(), r2 = col.getRadius();

		float x = std::abs(c1.x - c2.x), y = std::abs(c1.y - c2.y);
		float dist = std::sqrt(x * x + y * y);

		//collision detected
		if (dist < r1 + r2) {
			std::cout << "COL" << std::endl;
		}
	}

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
				bullets.erase(bullets.begin() + i);
				small.erase(small.begin() + j);
				b = 1;
				break;
			}
		}

		if (b) break;
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
