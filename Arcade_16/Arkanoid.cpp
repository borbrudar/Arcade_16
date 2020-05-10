#include "Arkanoid.h"

Arkanoid::Arkanoid(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14);

	//setup block
	for (int x = 0; x < sizeM; x++) {
		for (int y = 0; y < sizeN; y++) {
			blocks[x][y].setFillColor(Color::Green);
			blocks[x][y].setSize(Vector2f(sizeX, sizeY));
			blocks[x][y].setPosition(x * sizeX + offsetX + x * spacing, y * sizeY + offsetY + y * spacing);
		}
	}
	
}

void Arkanoid::draw(RenderWindow& window)
{
	for (int x = 0; x < sizeM; x++) {
		for (int y = 0; y < sizeN; y++) {
			window.draw(blocks[x][y]);
		}
	}
	back.draw(window);
}

void Arkanoid::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	//clicky boi
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
	}
	if (back.isClicked(mouse, window)) gameState = state::menu;

}
