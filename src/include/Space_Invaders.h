#pragma once
#include "State.h"
#include "Animation.h"
#include "Shield.h"
#include <vector>
#include <random>
#include <iostream>
#include <fstream>

constexpr int invdM = 10, invdN = 6;
const float space_scale = 3.f;

//projectile class
class Projectile {
public:
	Projectile() = default;
	Projectile(Texture t1, Vector2f pos) {
		setup(t1, pos);
	}
	void setup(Texture t1, Vector2f pos) {
		this->pos = pos;
		projectile.setTexture(t1);
		projectile.setScale(space_scale, space_scale);
		projectile.setPosition(pos);
	}
	void draw(RenderWindow& window) {
		pos = projectile.getPosition();
		window.draw(projectile);
	}
	void update(float delta){
		projectile.move(0, speedy * delta);
	}
	Sprite projectile;
	Vector2f pos;
	float speedy = -700.f;
};

//invader class
class Invader {
public:
	void setup(std::string string, Vector2f size, Vector2f number, Vector2f spacing, Vector2f off) {
		animation.setup(string, size);
		//wow such creativity you named 2 variables exactly the same...im impressed
		animation.animation.setPosition(number.x * (size.x * space_scale + spacing.x) + off.x,
			number.y * (size.y * space_scale + spacing.y) + off.y);
		animation.animation.setScale(Vector2f(space_scale, space_scale));
	}
	void draw(RenderWindow& window) {
		animation.draw(window);
	}
	Animation animation;
	float speedx = 50.f;
	bool alive = 1;
};

class Space_Invaders : public State {
public:
	//functions
	Space_Invaders(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e, float delta);

private:
	//utility shit
	std::vector < std::vector<Invader> > invaders;
	Button back;
	Sprite cannon;
	Texture c, proj, proj2, xp, xp2;
	bool gameOver = 0, transition = 0;
	int score = 0, highscore;
	Text scr;
	float cannonshoot = -700.f;
	std::fstream high;
	Sprite dlife;
	//////////////////////////////////////////
	// TURN RESET INTO A FUNCTION
	//////////////////////////////////////////
	//invader varbs
	Vector2f off{ 30,50 }; 
	Vector2f size1{ 11,8 }, size2{ 8, 8 }, size3{ 12,8 }, cSize{ 13,8 }, bSize{ 1,4 }, dSize{ 15,10 };
	Vector2f spacing1{ 8,5 }, spacing2{ 17,5 }, spacing3{ 5,5 };
	bool inThere = 1;
	//cannon varbs
	float cannonx = 0.f;
	bool left = 0, right = 0, shoot = 0;
	float speed = 300.f;
	int lives = 3;

	Animation death;
	Clock deathC;
	float dtimer = 0.f, dtime = 0.f, ddelay = 1.f;

	//tic toc
	Clock bClock;
	float timer = 0, time = 0, delay = 0.3f;

	//explosion
	std::vector<Sprite> explosions;
	std::vector<float> xptimer;
	Clock xpClock;
	float xptime = 0.f, xpdelay = 0.2f;

	//projectile
	std::vector<Projectile> bullets;

	std::vector<int> alienI;
	std::vector<Projectile> alienBullets;
	Clock aClock;
	float atime = 0.f, atimer = 0.f, adelay = 0.3f;

	//shield
	std::vector<Shield> shields;
};