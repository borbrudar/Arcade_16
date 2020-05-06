#include "Tetris.h"

Tetris::Tetris(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20)); 

	//setup tiles
	til.loadFromFile("res/tetris/tiles.png");
	tiles.setTexture(til);
	tiles.setTextureRect(IntRect(0, 0, 18, 18));
}

void Tetris::draw(RenderWindow& window)
{
	//draw button
	back.draw(window);

	//draw tiles
	for (int i = 0; i < 4; i++) {
		tiles.setPosition(a[i].x * 18, a[i].y * 18);
		window.draw(tiles);
	}
}

void Tetris::update(Mouse& mouse, RenderWindow& window, state& gameState, Event &e)
{
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (e.type == Event::KeyPressed) {
			if (e.key.code == Keyboard::Up) rot = 1;
			else if (e.key.code == Keyboard::Left) dx = -1;
			else if (e.key.code == Keyboard::Right) dx = 1;
		}
	}
	if (back.isClicked(mouse,window)) gameState = state::menu;
	
	//check for border
	if(dx == 1)
	for (int i = 0; i < 4; i++) if (a[i].x == 9) dx = 0;
	
	if (dx == -1)
		for (int i = 0; i < 4; i++) if (a[i].x == 0) dx = 0;

	//rotation
	if(rot){
		point p = a[1]; //center of rotation
		for (int i = 0; i < 4; i++) {
			int x = a[i].y - p.y;
			int y = a[i].x - p.x;
			a[i].x = p.x - x;
			a[i].y = p.y + y;
		}
	}

	//move
	for (int i = 0; i < 4; i++) a[i].x += dx;

	//set figures
	int n = 3;
	if (a[0].x == 0) {
		for (int i = 0; i < 4; i++) {
			a[i].x = shapes[n][i] % 2;
			a[i].y = shapes[n][i] / 2;
		}
	}

	dx = 0; rot = 0;
}
