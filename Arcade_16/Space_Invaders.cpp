#include "Space_Invaders.h"

Space_Invaders::Space_Invaders(Font& f)
{
	//back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(40, 25), Vector2f(580, 2), 12);

	//invaders
	invaders.resize(invdM);
	for (int x = 0; x < invdM; x++) {
		invaders[x].resize(invdN);
		for (int y = 0; y < invdN; y++) {
			invaders[x][y].setFillColor(Color::White);
			invaders[x][y].setSize(Vector2f(sizeX, sizeY));
			invaders[x][y].setPosition(x * (sizeX + spacing) + offsetX, y * (sizeY + spacing) + offsetY);
		}
	}
}

void Space_Invaders::draw(RenderWindow& window)
{
	back.draw(window);

	for (int x = 0; x < invdM; x++) {
		for (int y = 0; y < invdN; y++) window.draw(invaders[x][y]);
	}
}

void Space_Invaders::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
	}
	if (back.isClicked(mouse, window)) gameState = state::menu;
}
