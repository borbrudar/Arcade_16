#include "Menu.h"


Menu::Menu(Font& f)
{
	std::string text1;
	//tetris
	text1.assign("1.Tetris");
	tetris.setup(f, text1, Color::Green, Vector2f(200, 50), Vector2f(70, 90), 22);
	//arkanoid
	text1.assign("2. Arkanoid");
	arkanoid.setup(f, text1, Color::Green, Vector2f(200, 50), Vector2f(70, 170), 22);

	//space invaders
	text1.assign("3. Space Invaders");
	spaceinvaders.setup(f, text1, Color::Green, Vector2f(200, 50), Vector2f(70, 250), 22);

	//pong
	text1.assign("4. Pong");
	pong.setup(f, text1, Color::Green, Vector2f(200, 50), Vector2f(70, 330), 22);

	//asteroids
	text1.assign("5. Asteroids");
	asteroids.setup(f, text1, Color::Green, Vector2f(200, 50), Vector2f(300, 90), 22);

	//pac man
	text1.assign("6.Pac Man");
	pacman.setup(f, text1, Color::Green, Vector2f(200, 50), Vector2f(300, 170), 22);

	//simon
	text1.assign("7.Simon");
	simon.setup(f, text1, Color::Green, Vector2f(200, 50), Vector2f(300, 250), 22);

	//super mario
	text1.assign("8.Super Mario");
	supermario.setup(f, text1, Color::Green, Vector2f(200, 50), Vector2f(300, 330), 22);


	//arcade 
	arcade.setCharacterSize(50);
	arcade.setFillColor(Color::Green);
	arcade.setPosition(180, 10);
	arcade.setOutlineColor(Color::Black);
	arcade.setOutlineThickness(3.5f);
	arcade.setString("Arcade 16");
	arcade.setFont(f);
}


void Menu::draw(RenderWindow &window)
{ 
	window.draw(arcade);

	tetris.draw(window);
	arkanoid.draw(window);
	spaceinvaders.draw(window);
	pong.draw(window);
	asteroids.draw(window);
	pacman.draw(window);
	simon.draw(window);
	supermario.draw(window);
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
	if (supermario.isClicked(mouse, window)) gameState = state::super_mario;
}
