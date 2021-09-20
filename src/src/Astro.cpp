#include "Astro.h"

Astro::Astro(int size, Texture &tex, Vector2f pos)
{
	ast.setTexture(&tex);

	float r = 0;
	if (size == 0) r = 40.f;
	else if (size == 1) r = 20.f;
	else if (size == 2) r = 10.f;

	this->r = r;
	ast.setRadius(r);
	ast.setOrigin(r / 2, r / 2);
	ast.setPosition(pos);

	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<float> dist(0, 1);

	vel = Vector2f(dist(engine) / 5, dist(engine) / 5);
	rot = dist(engine) / 5;
}

void Astro::draw(RenderWindow& window)
{
	window.draw(ast);
}

void Astro::update() {
	Vector2f pos = ast.getPosition();

	if (pos.x < 0) ast.setPosition(scrWidth, ast.getPosition().y);
	if (pos.x > scrWidth) ast.setPosition(0, ast.getPosition().y);
	if (pos.y < 0) ast.setPosition(ast.getPosition().x, scrHeight);
	if (pos.y > scrHeight) ast.setPosition(ast.getPosition().x, 0);

	ast.move(vel);
	ast.rotate(rot);
}