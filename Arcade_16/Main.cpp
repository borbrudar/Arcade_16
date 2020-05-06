#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Tetris.h"
#include "stateEnum.h"

const int scrWidth = 640, scrHeight = scrWidth / 4 * 3;
using namespace sf;

int main() {
	//setup
	RenderWindow window;
	window.create(VideoMode(scrWidth, scrHeight), "Arcade 16");
	Event e;
	Mouse mouse;
	
	Font arial;
	arial.loadFromFile("res/font/arial.ttf");

	state gameState = state::menu;
	state prevState = state::menu;
	std::unique_ptr<State> state = std::make_unique<Tetris>(arial);

	//game loop
	while (window.isOpen()) {
		//switch state
		if (prevState != gameState) {
			prevState = gameState;

			switch (gameState) {
			case state::menu:
				state = std::make_unique<Menu>(arial);
				break;
			case state::tetris:
				state = std::make_unique<Tetris>(arial);
				break;
			}
		}
		//update (event check etc)
		state->update(mouse, window, gameState, e);
		////////////////////////////////////////
		window.clear(Color(126,126,126));

		state->draw(window);

		window.display();
		////////////////////////////////////////
	}
	
	return 0;
}