#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Tetris.h"
#include "Arkanoid.h"
#include "Space_Invaders.h"
#include "stateEnum.h"
#include "Pong.h"

using namespace sf;

int main() {
	//setup
	RenderWindow window;
	window.create(VideoMode(scrWidth, scrHeight), "Arcade 16");
	Event e;
	Mouse mouse;

	Font arial;
	arial.loadFromFile("res/font/arial.ttf");

	state gameState = state::pong;
	state prevState = gameState;
	std::unique_ptr<State> state = std::make_unique<Pong>(arial); //FOR TEST YOU HAVE TO SWITCH THE TOP ONES TOO!!!


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
			case state::arkanoid:
				state = std::make_unique<Arkanoid>(arial);
				break;
			case state::space_invaders:
				state = std::make_unique<Space_Invaders>(arial);
				break;
			case state::pong:
				state = std::make_unique<Pong>(arial);
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