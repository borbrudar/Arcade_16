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

	state gameState = state::tetris;
	state prevState = state::tetris;
	std::unique_ptr<State> state = std::make_unique<Tetris>(arial); //FOR TEST YOU HAVE TO SWITCH THE TOP ONES TOO!!!

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
		window.clear(Color(50,50,50));

		state->draw(window);

		window.display();
		////////////////////////////////////////
	}
	
	//deletes the pointer and calls appropriate destructor
	state.reset();
	return 0;
}