#pragma once
#include "State.h"
#include "Animation.h"
#include <vector>

constexpr int invdM = 10, invdN = 6;
const float space_scale = 3.f;

//projectile class
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
	float speedy = -1.f;
};

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
	float speedx = 0.2f;
	bool alive = 1;
};

class Space_Invaders : public State {
public:
	//functions
	Space_Invaders(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

private:
	//utility shit
	std::vector < std::vector<Invader> > invaders;
	Button back;
	Sprite cannon;
	Texture c, proj;

	//inside variables (should abstract into an invader class but who gives a fuck)
	Vector2f off{ 30,50 }; 
	Vector2f size1{ 11,8 }, size2{ 8, 8 }, size3{ 12,8 }, cSize{ 13,8 }, bSize{ 1,5 };
	Vector2f spacing1{ 8,5 }, spacing2{ 17,5 }, spacing3{ 5,5 };
	float cannonx = 0.f;
	bool left = 0, right = 0, shoot = 0;
	float speedL = -0.4f, speedR = 0.4f;
	//tic toc
	Clock bClock;
	float timer = 0, time = 0, delay = 0.2f;

	//projectile
	std::vector<Projectile> bullets;
};