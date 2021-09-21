#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "stateEnum.h"
//GAMES
#include "Tetris.h"
#include "Arkanoid.h"
#include "Space_Invaders.h"
#include "Pong.h"
#include "Asteroids.h"
#include "Pac_Man.h"
#include "Simon.h"
#include "Super_Mario.h"

using namespace sf;

int main() {
	//setup
	RenderWindow window;
	window.create(VideoMode(scrWidth, scrHeight), "Arcade 16");
	Event e;
	Mouse mouse;

	//resources
	Font arial, mario;
	arial.loadFromFile("res/font/arial.ttf");
	mario.loadFromFile("res/font/mario.ttf");
	
	//game state
	state gameState = state::menu;
	state prevState = gameState;
	std::unique_ptr<State> state = std::make_unique<Menu>(arial); 

	//timestep
	sf::Clock clock;
	double dt = 1 / 60.f, currentTime = clock.getElapsedTime().asSeconds();

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
			case state::asteroids:
				state = std::make_unique<Asteroids>(arial);
				break;
			case state::pacman:
				state = std::make_unique<Pac_Man>(arial);
				break;
			case state::simon:
				state = std::make_unique<Simon>(arial);
				break;
			case state::super_mario:
				state = std::make_unique<Super_Mario>(mario);
				break;
			}
		}
	
		//timestep
		double newTime = clock.getElapsedTime().asSeconds();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		while (frameTime > 0.0)
		{
		float deltaTime = std::min(frameTime, dt);
		//update (event check etc)
		state->update(mouse, window, gameState, e, deltaTime);
		frameTime -= deltaTime;
		}
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