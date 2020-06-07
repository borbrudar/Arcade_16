#include "Ghost.h"

void Ghost::setup(int type, Vector2f pos, Vector2f size, Vector2f start, std::vector<std::vector<int>> field)
{
	this->start = start;
	this->type = type;
	this->tSize = size;
	this->field = field;
	animation.animation.setScale(scale);
	animation.delay = 0.4f;
	this->pos = Vector2f(pos.x * size.x + start.x, pos.y * size.y + start.y);

	/*for (int x = 0; x < this->field.size(); x++) {
		for (int y = 0; y < this->field.size(); y++) {
			std::cout << field[x][y] << " ";
		}
		std::cout << std::endl;
	}*/
}

void Ghost::draw(RenderWindow& window)
{
	animation.animation.setPosition(pos);
	animation.draw(window);

}

void Ghost::update(Vector2f player)
{
	//normalize coords to integers
	Vector2i pac = Vector2i((std::round((player.x - start.x) / tSize.x)) , (std::round((player.y - start.y) / tSize.y)));
	Vector2i pos = Vector2i(std::round((this->pos.x - start.x) / tSize.x), std::round((this->pos.y - start.y)/ tSize.y));
	
	if(beg) {
		findPath(pac, pos); beg = 0;
	}
	//if move done find next target
	if (move(pos)) { findPath(pac, pos); }

}

void Ghost::findPath(Vector2i target, Vector2i curPos)
{
	//check all the neighbours
	std::vector<float> neighbours;

	//check if out of bounds and add to the list
	// 
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

bool Ghost::move(Vector2i curPos)
{
	switch (instruction) {
	case 0:
		//right
		animation.setup("res/pacman/gh.png", size, Vector2f(size.x * 6, 0));
		pos.x += speed;
		break;
	case 1: 
		//left
		animation.setup("res/pacman/gh.png", size, Vector2f(size.x * 4, 0));
		pos.x -= speed;
		break;
	case 2:
		//down
		animation.setup("res/pacman/gh.png", size, Vector2f(size.x * 2,0));
		pos.y += speed;
		break;
	case 3:
		//up
		animation.setup("res/pacman/gh.png", size);
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
