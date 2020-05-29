#include "Astro.h"

void Astro::setup(int type, int size, Vector2f pos)
{
	if (size == 0) {
		if (!tex.loadFromFile("res/asteroids/big_ast1.png")) std::cout << "EROR";
	}

	ast.setTexture(&tex);
	
	float r = 0;
	if (size == 0) r = 25.f;
	else if (size == 1) r = 12.5f;
	else if (size == 2) r = 6.25f;

	ast.setRadius(r);
	ast.setOrigin(r/2, r/2);
	ast.setPosition(pos);
	
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<float> dist(0, 1);

	vel = Vector2f(dist(engine) / 5, dist(engine) / 5);
	rot = dist(engine) / 5;
}

void Astro::draw(RenderWindow& window)
{
	Vector2f pos = ast.getPosition();

	if (pos.x < 0) ast.setPosition(scrWidth, ast.getPosition().y);
	if (pos.x > scrWidth) ast.setPosition(0, ast.getPosition().y);
	if (pos.y < 0) ast.setPosition(ast.getPosition().x, scrHeight);
	if (pos.y > scrHeight) ast.setPosition(ast.getPosition().x, 0);

	ast.move(vel);
	ast.rotate(rot);

	window.draw(ast);
}