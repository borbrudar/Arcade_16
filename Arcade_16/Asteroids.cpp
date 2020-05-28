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
}

void Asteroids::draw(RenderWindow& window)
{
	window.clear(Color::Black);
	back.draw(window);

	window.draw(ship);

	for (int i = 0; i < bullets.size(); i++) bullets[i].draw(window);
}

void Asteroids::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	//click click
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (e.type == Event::KeyPressed) {
			if (e.key.code == Keyboard::Left) left = 1;
			else if (e.key.code == Keyboard::Right) right = 1;
			if (e.key.code == Keyboard::Space) shoot = 1;
		}
		if (e.type == Event::KeyReleased) {
			if (e.key.code == Keyboard::Left) left = 0;
			if (e.key.code == Keyboard::Right) right = 0;
			if (e.key.code == Keyboard::Space) shoot = 0;
		}
	}
	if (back.isClicked(mouse, window)) gameState = state::menu;
	
	//ship rot
	if (left) rot = -rots;
	else if (right)	rot = rots;
	else rot = 0;
	ship.rotate(rot);


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

	//erase bullets which are offscreen
	for (int i = 0; i < bullets.size(); i++) if (bullets[i].isOffScreen()) bullets.erase(bullets.begin() + i);
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
