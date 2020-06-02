#include "Ghost.h"

void Ghost::setup(int type, Vector2f pos, Vector2f size, Vector2f start, std::vector<std::vector<int>> field)
{
	this->start = start;
	this->type = type;
	this->field = field;
	this->tSize = size;
	up.setup("res/pacman/gh.png", this->size);
	up.animation.setScale(scale);
	this->pos = Vector2f(pos.x * size.x + start.x, pos.y * size.y + start.y);
}

void Ghost::draw(RenderWindow& window)
{
	up.animation.setPosition(pos);
	up.draw(window);

	for(int i = 0; i < test.size();i++) window.draw(test[i]);
}

void Ghost::update(Vector2f player)
{
	//remove multipliers, add pathfinding alg
	Vector2f target = Vector2f((std::round((player.x) / size.x) ), (std::round((player.y) / size.y)));
	Vector2f pos = Vector2f(std::round(this->pos.x / size.x), std::round(this->pos.y/ size.y));

	if (pos.x >= target.x) ud = 0; else ud = 1;
	if (pos.y >= target.y) lr = 0; else lr = 1;

	findPath(player, pos);


	test.resize(path.size());
	for (int i = 0; i < test.size(); i++) {
		test[i].setFillColor(Color(0, 0, 0, 0));
		test[i].setOutlineColor(Color::Red);
		test[i].setOutlineThickness(2.f);
		test[i].setSize(tSize);
		
		test[i].setPosition(path[i]);
	}

}

void Ghost::findPath(Vector2f target, Vector2f curPos)
{
	if (curPos == target) {
		foundPath = 1; return;
	}

	//pathfind
	int way = 0;
	if (way == 0) {
		if (curPos.x + 1 < sfield.x) {
			if (field[curPos.x + 1][curPos.y] != 1) {
				path.push_back(Vector2f(curPos));
				findPath(target, Vector2f(curPos.x + 1, curPos.y));
			}
			else way++;
		}
		else way++;
	}
	else way++;
	if (way == 1 ) {
		if ((curPos.x - 1) >= 0) {
			if (field[curPos.x - 1][curPos.y] != 1) {
				path.push_back(Vector2f(curPos));
				findPath(target, Vector2f(curPos.x - 1, curPos.y));
			}
			else way++;
		}
		else way++;
	}
	else way++;
	if (way == 2 ) {
		if (curPos.y + 1 < sfield.y) {
			if (field[curPos.x][curPos.y + 1] != 1) {
				path.push_back(Vector2f(curPos));
				findPath(target, Vector2f(curPos.x, curPos.y + 1));
			}
			else way++;
		}
		else way++;
	}
	else way++;
	if (way == 3 ) {
		if (curPos.y - 1 >= 0) {
			if (field[curPos.x][curPos.y - 1] != 1) {
				path.push_back(Vector2f(curPos));
				findPath(target, Vector2f(curPos.x, curPos.y - 1));
			}
		}
	}
	else way++;

	if (foundPath) return;
	if(path.size() > 0) path.pop_back();
}
