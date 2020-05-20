#pragma once
#include "State.h"
#include "Animation.h"
#include <vector>

constexpr int invdM = 10, invdN = 6;
const float space_scale = 3.f;

class Projectile {
public:
	Projectile(Texture t1, Vector2f pos) : pos(pos) {
		projectile.setTexture(t1);
		projectile.setScale(space_scale, space_scale);
		projectile.setPosition(pos);
	}
	void draw(RenderWindow& window) {
		projectile.move(0, speedy);
		pos = projectile.getPosition();
		window.draw(projectile);
	}
	Sprite projectile;
	Vector2f pos;
private:
	float speedy = -0.5f;
};

class Space_Invaders : public State {
public:
	//functions
	Space_Invaders(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

private:
	//utility shit
	std::vector < std::vector<Animation> > invaders;
	Button back;
	Sprite cannon;
	Texture c, proj;

	//inside variables
	int offsetX = 30, offsetY = 50;
	Vector2f size1{ 11,8 }, size2{ 8, 8 }, size3{ 12,8 }, cSize{ 13,8 }, bSize{ 1,5 };
	Vector2f spacing1{ 8,5 }, spacing2{ 17,5 }, spacing3{ 5,5 };
	float speedx = 0.2f, cannonx = 0.f;
	//tic toc
	Clock bClock;
	float timer = 0, time = 0, delay = 0.35f;

	//projectile
	std::vector<Projectile> bullets;
};