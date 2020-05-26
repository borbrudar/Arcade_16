#include "Menu.h"


Menu::Menu(Font& f)
{
	//tetris
	std::string text1;
	text1.assign("1.Tetris");
	tetris.setup(f, text1, Color::Green, Vector2f(350, 50), Vector2f(70, 50), 24);
	//arkanoid
	text1.assign("2. Arkanoid");
	arkanoid.setup(f, text1, Color::Green, Vector2f(350, 50), Vector2f(70, 140), 24);

	//space invaders
	text1.assign("3. Space Invaders");
	spaceinvaders.setup(f, text1, Color::Green, Vector2f(350, 50), Vector2f(70, 220), 24);

	//pong
	text1.assign("4. Pong");
	pong.setup(f, text1, Color::Green, Vector2f(350, 50), Vector2f(70, 300), 24);
}


void Menu::draw(RenderWindow &window)
{ 
	tetris.draw(window);
	arkanoid.draw(window);
	spaceinvaders.draw(window);
	pong.draw(window);
}

void Menu::update(Mouse &mouse, RenderWindow& window, state &gameState, Event &e)
{
	//event check
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
	}

	if (tetris.isClicked(mouse, window)) gameState = state::tetris;
	if (arkanoid.isClicked(mouse, window)) gameState = state::arkanoid;
	if (spaceinvaders.isClicked(mouse, window)) gameState = state::space_invaders;
	if (pong.isClicked(mouse, window)) gameState = state::pong;
}
