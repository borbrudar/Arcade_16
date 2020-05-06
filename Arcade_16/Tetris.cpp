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
	}

	if (back.isClicked(mouse,window)) gameState = state::menu;
	
	//set figures
	int n = 5;
	for (int i = 0; i < 4; i++) {
		a[i].x = shapes[n][i] % 2;
		a[i].y = shapes[n][i] / 2;
	}
}
