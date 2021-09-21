#pragma once
#include "State.h"
#include "Mario.h"
#include "Box.h"
#include "Enemy.h"
#include "Entity.h"
#include "BoxEnum.h"
#include "MP.h"
#include <fstream>
#include <SFML/Audio.hpp>

struct Explosion {
	Explosion(Vector2f pos, Texture& t1, Vector2f eSize, Vector2f tSize, int maxSwap = 2):
	maxSwap(maxSwap), pos(pos) {
		box.setup(t1, eSize, tSize, maxSwap);
		box.animation.setPosition(pos);
		box.delay = 0.25f;
	}
	void draw(RenderWindow& window) {
		box.draw(window);
	}
	bool update() {
		box.animation.setPosition(pos.x + offX + oddX, pos.y);
		if (box.getSwap() == maxSwap) return 1;
		return 0;
	}
	void off(int x) {
		offX = x;
	}

	int maxSwap, offX = 0, oddX = 0;
	Manimation box;
	Vector2f pos;
};

struct Broken {
	Broken(Texture &t, Vector2f tSize, Vector2f pos, bool minus = 0) :	pos(pos) {
		temp.setup(t, Vector2f(16, 16), Vector2f(tSize.x / 4, tSize.y/4), 0);
		temp.setCycle(0);
		temp.setSwap(1);
		if (minus) speedx = -speedx;
		temp.animation.setRotation(45);
		temp.animation.setPosition(pos);
	}
	void draw(RenderWindow& window) {
		temp.draw(window);
	}
	bool update() {
		pos.x += speedx;
		pos.y += speedy;
		temp.animation.setPosition(pos);
		if (temp.animation.getPosition().x > scrWidth || temp.animation.getPosition().x < 0 ||
			temp.animation.getPosition().y > scrHeight || temp.animation.getPosition().y < 0) return 1;
		return 0;
	}
	void off(int x) {
		offX = x;
	}

	Manimation temp;
	Vector2f pos;
	bool minus = 0;
	float offX = 0.f, oddX = 0.f, speedy = 1.2f, speedx = .6f;
};

class Super_Mario : public State {
public:
	Super_Mario(Font& f);
	~Super_Mario();
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e, float delta);
	void loadWorld(int start, int end, bool plus = 1);
	void deleteWorld();
	void drawString(int x, int y, std::string string, RenderWindow &window);

	Button back;
	//sound
	Music theme;
	SoundBuffer bl_br, appear, powerup, death, coin, fire, jump;
	Sound sound;
	bool th = 1, jp = 0;

	//level
	Image lvl;
	Texture gr, br, cl, h1;
	float tx = 0.f;
	int lastX = 0, firstX = -7; // -7 to accomodate for 5 block long visuals, and 2 just for it to work
	bool gameOver = 0;

	//score stuff
	int score = 0, highscore;
	Text scr;
	std::fstream high;
	Font mfont;

	//time
	int times = 400;
	Clock clock;
	float time = 0, timer = 0, delay = 1.f;

	//visual
	Vector2f c1{ 32,24 }, c2{ 48,24 }, c3{ 64,24 }, c4{ 80,35 };
	std::vector<Broken> broken;

	//phyiscs
	std::vector<Box> boxes;
	std::vector<Box> blocks;
	std::vector<Box> coins_;
	Vector2f off{ 0,20 }, tSize, bSize{ 16,16 };
	float offX = 0.f;
	int coins = 0;

	//mario
	Mario mario;
	bool left = 0, right = 0, up = 0, sprint = 0;
	std::vector<Vector2f> mSize{ {17,16}, {16,32} };
	std::vector<Texture> mr;
	int lives = 1;

	//his projectiles
	Texture proj, xp;
	std::vector<MP> mp;
	Vector2f mpSize{ 8,8 };
	std::vector<Explosion> xps; //xps rely on bSize

	//enemies
	std::vector<Enemy> enemies;
	Vector2f e1Size{ 16,16 }, e2Size{ 16,24 };
	Texture en, en2;

	//entity
	std::vector<Entity> entities;
	Texture tits, schrooms;
	Vector2f cSize{ 10,14 }, sSize{ 16,16 };
};