#pragma once
#include "State.h"
#include "Animation.h"
#include "Ghost.h"
#include "Pellet.h"

class Wall {
public:
	Wall(Vector2f pos, Vector2f size, Texture &tex, Vector2f start,int rot = 0) {
		wall.setTexture(&tex);
		wall.setPosition(pos.x + start.x, pos.y + start.y);
		wall.setSize(size);

		if (rot == 90) { wall.setOrigin(0 , size.y); }
		if (rot == 180) wall.setOrigin(size.x, size.y);
		if (rot == 270) wall.setOrigin(size.x, 0);
		wall.setRotation(rot);
	};
	RectangleShape wall;
};


class Pac_Man : public State {
public:
	Pac_Man(Font& f);
	void draw(RenderWindow& window);
	void update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e);

private:
	Button back;
	Vector2f start{ 150, 20 };
	std::vector<std::vector<int>> field;

	//pacman
	Animation pacman;
	bool up = 0, down = 0, left = 0, right = 0;
	float speed = 0.8f, pscale = 0.2f; 
	Vector2f pSize{ 80,80 }, prevPos;
	Vector2i tSize;

	//walls
	Texture pp, st, cp, cr, inv;
	Image lvl;
	std::vector<Wall> walls;

	//ghosts
	Ghost blinky, pinky, inky, clyde;

	//pellets
	std::vector<Pellet> pellets;
};