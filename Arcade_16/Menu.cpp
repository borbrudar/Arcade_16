#include "Menu.h"


void Menu::draw(RenderWindow &window)
{ 
	button.draw(window);
}

void Menu::update(Mouse &mouse, RenderWindow& window, state &gameState, Event &e)
{
	//event check
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
	}

	if (button.isClicked(mouse, window)) gameState = state::tetris;

}
