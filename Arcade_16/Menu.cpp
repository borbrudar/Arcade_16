#include "Menu.h"


Menu::Menu(Font& f)
{
	//tetris
	std::string text1;
	text1.assign("1.Tetris");
	tetris.setup(f, text1, Color::Green, Vector2f(350, 40), Vector2f(70, 30), 22);
	//arkanoid
	text1.assign("2. Arkanoid");
	arkanoid.setup(f, text1, Color::Green, Vector2f(350, 40), Vector2f(70, 90), 22);

	//space invaders
	text1.assign("3. Space Invaders");
	spaceinvaders.setup(f, text1, Color::Green, Vector2f(350, 40), Vector2f(70, 150), 22);

	//pong
	text1.assign("4. Pong");
	pong.setup(f, text1, Color::Green, Vector2f(350, 40), Vector2f(70, 210), 22);

	//asteroids
	text1.assign("5. Asteroids");
	asteroids.setup(f, text1, Color::Green, Vector2f(350, 40), Vector2f(70, 270), 22);

	//pac man
	text1.assign("6.Pac Man");
	pacman.setup(f, text1, Color::Green, Vector2f(350, 40), Vector2f(70, 330), 22);

	//simon
	text1.assign("7.Simon");
	simon.setup(f, text1, Color::Green, Vector2f(350, 40), Vector2f(70, 390), 22);

}


void Menu::draw(RenderWindow &window)
{ 
	tetris.draw(window);
	arkanoid.draw(window);
	spaceinvaders.draw(window);
	pong.draw(window);
	asteroids.draw(window);
	pacman.draw(window);
	simon.draw(window);
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
	if (asteroids.isClicked(mouse, window)) gameState = state::asteroids;
	if (pacman.isClicked(mouse, window)) gameState = state::pacman;
	if (simon.isClicked(mouse, window)) gameState = state::simon;
}
