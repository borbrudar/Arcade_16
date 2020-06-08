#include "Ghost.h"

void Ghost::setup(int type, Vector2f pos, Vector2f size, Vector2f start, std::vector<std::vector<int>> field)
{
	this->start = start;
	this->type = type;
	this->tSize = size;
	this->field = field;
	animation.animation.setScale(scale);
	this->pos = Vector2f(pos.x * size.x + start.x, pos.y * size.y + start.y);

}

void Ghost::draw(RenderWindow& window)
{
	animation.animation.setPosition(pos);
	animation.draw(window);
}

//blinky and clyde
void Ghost::update(Vector2f player)
{
	//normalize coords to integers
	Vector2i tar = Vector2i((std::round((player.x - start.x) / tSize.x)), (std::round((player.y - start.y) / tSize.y)));
	Vector2i pos = Vector2i(std::round((this->pos.x - start.x) / tSize.x), std::round((this->pos.y - start.y)/ tSize.y));

	if(beg) {
		findPath(tar, pos); beg = 0;
	}

	//blinky
	if (type == 0) {
		//if move done find next target
		if (move()) { findPath(tar, pos); }
	}//idiot clyde

	else if (type == 3) {
		float dist = std::sqrt(std::pow(pos.x - tar.x, 2) + std::pow(pos.y - tar.y, 2));

		if (dist > 4) {
			if (move()) findPath(tar, pos); 
		}
		else {
			//else run to the nearest corner
			std::vector<Vector2i> c{ {1,1},{ 18,1 },{ 1,21 },{ 18,21 } }; 
			std::vector<float> dists;
			dists.push_back(std::sqrt(std::pow(pos.x - c[0].x, 2) + std::pow(pos.y - c[0].y, 2)));
			dists.push_back(std::sqrt(std::pow(pos.x - c[1].x, 2) + std::pow(pos.y - c[1].y, 2)));
			dists.push_back(std::sqrt(std::pow(pos.x - c[2].x, 2) + std::pow(pos.y - c[2].y, 2)));
			dists.push_back(std::sqrt(std::pow(pos.x - c[3].x, 2) + std::pow(pos.y - c[3].y, 2)));

			float temp = 99999;
			for (int i = 0; i < dists.size(); i++) {
				if (dists[i] < temp) {
					temp = dists[i];
					tar = c[i];
				}
			}

			if (move()) findPath(tar, pos);
		}
	}
}

//pinky
void Ghost::update(Vector2f player, int rot)
{
	//normalize coords to integers
	Vector2i tar;
	Vector2i pos = Vector2i(std::round((this->pos.x - start.x) / tSize.x), std::round((this->pos.y - start.y) / tSize.y));

	switch (rot) {
	case 0:
		tar = Vector2i((std::round((player.x - start.x) / tSize.x)) + 2, (std::round((player.y - start.y) / tSize.y)));
		break;
	case 180:
		tar = Vector2i((std::round((player.x - start.x) / tSize.x)) - 2, (std::round((player.y - start.y) / tSize.y)));
		break;
	case 90:
		tar = Vector2i((std::round((player.x - start.x) / tSize.x)), (std::round((player.y - start.y) / tSize.y)) + 2);
		break;
	case 270:
		tar = Vector2i((std::round((player.x - start.x) / tSize.x)), (std::round((player.y - start.y) / tSize.y)) - 2);
		break;
	}
	
	if (beg) {
		findPath(tar, pos); beg = 0;
	}
	//if move done find next target
	if (move()) { findPath(tar, pos); }
}

//inky
void Ghost::update(Vector2f player, int rot, Vector2f blinky)
{
	//normalize coords to integers
	Vector2i tar, origin;
	Vector2i pos = Vector2i(std::round((this->pos.x - start.x) / tSize.x), std::round((this->pos.y - start.y) / tSize.y));
	Vector2i bli = Vector2i(std::round((blinky.x - start.x) / tSize.x), std::round((blinky.y - start.y) / tSize.y));

	switch (rot) {
	case 0:
		origin = Vector2i((std::round((player.x - start.x) / tSize.x)) + 1, (std::round((player.y - start.y) / tSize.y)));
		break;
	case 180:
		origin = Vector2i((std::round((player.x - start.x) / tSize.x)) - 1, (std::round((player.y - start.y) / tSize.y)));
		break;
	case 90:
		origin = Vector2i((std::round((player.x - start.x) / tSize.x)), (std::round((player.y - start.y) / tSize.y)) + 1);
		break;
	case 270:
		origin = Vector2i((std::round((player.x - start.x) / tSize.x)), (std::round((player.y - start.y) / tSize.y)) - 1);
		break;
	}

	//rotate 180 degress around the point to find the new point and stuff
	float angle = 180.f;
	tar.x = ((bli.x - origin.x) * std::cos(angle)) - ((origin.y - bli.y) * std::sin(angle)) + origin.x;
	tar.y = ((origin.y - bli.y) * std::cos(angle)) - ((bli.x - origin.x) * std::sin(angle)) + origin.y;


	if (beg) {
		findPath(tar, pos); beg = 0;
	}

	//if move done find next target
	if (move()) { findPath(tar, pos); }
}



void Ghost::findPath(Vector2i target, Vector2i curPos)
{
	//TODO:
	//manually check if in the "exit"
	//and make him go the other way
	//until he is out


	//check all the neighbours
	std::vector<float> neighbours;

	//check if out of bounds and add to the list
	// alert no out of bounds exceptions and shit
	  //right
	if (field[curPos.x + 1][curPos.y] != 1 && Vector2i(curPos.x + 1, curPos.y) != prevPos) {
		neighbours.push_back(std::sqrt((target.x - curPos.x - 1) * (target.x - curPos.x - 1) +
			(target.y - curPos.y) * (target.y - curPos.y)));
	}
	else neighbours.push_back(99999);
	//left
	if (field[curPos.x - 1][curPos.y] != 1 && Vector2i(curPos.x - 1, curPos.y) != prevPos) {
		neighbours.push_back(std::sqrt((target.x - curPos.x + 1) * (target.x - curPos.x + 1) +
			(target.y - curPos.y) * (target.y - curPos.y)));
	}
	else neighbours.push_back(99999);
	//down
	if (field[curPos.x][curPos.y + 1] != 1 && Vector2i(curPos.x, curPos.y + 1) != prevPos) {
		neighbours.push_back(std::sqrt((target.x - curPos.x) * (target.x - curPos.x) +
			(target.y - curPos.y - 1) * (target.y - curPos.y - 1)));
	}
	else neighbours.push_back(99999);
	//up
	if (field[curPos.x][curPos.y - 1] != 1 && Vector2i(curPos.x, curPos.y - 1) != prevPos) {
		neighbours.push_back(std::sqrt((target.x - curPos.x) * (target.x - curPos.x) + 
			(target.y - curPos.y + 1) * (target.y - curPos.y + 1)));
	}
	else neighbours.push_back(99999);


	//choose the one with lowest value and add to instructions
	float lowest = 99999999;
	for (int i = 0; i < neighbours.size(); i++) {
		if (neighbours[i] < lowest) {
			lowest = neighbours[i];
			instruction = i;
		}
	}

	prevPos = curPos;
}

bool Ghost::move()
{
	switch (instruction) {
	case 0:
		//right
		animation.setup("res/pacman/gh.png", size, Vector2f(size.x * 6, type * size.y));
		pos.x += speed;
		break;
	case 1: 
		//left
		animation.setup("res/pacman/gh.png", size, Vector2f(size.x * 4, type * size.y));
		pos.x -= speed;
		break;
	case 2:
		//down
		animation.setup("res/pacman/gh.png", size, Vector2f(size.x * 2, type * size.y));
		pos.y += speed;
		break;
	case 3:
		//up
		animation.setup("res/pacman/gh.png", size, Vector2f(0, type * size.y));
		pos.y -= speed;
		break;
	}


	distTraveled += speed;

	if (distTraveled > tSize.x) {
		distTraveled = 0.f;
		return 1;
	}
	return 0;
}
