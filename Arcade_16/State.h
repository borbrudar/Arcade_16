#pragma once
#include <SFML/Graphics.hpp>
#include "stateEnum.h"
#include "Button.h"

const int scrWidth = 640, scrHeight = scrWidth / 4 * 3;
using namespace sf;

class State {
public:
	virtual void draw(RenderWindow &window) = 0;
	virtual void update(Mouse &mouse, RenderWindow& window, state &gameState, Event &e) = 0;
	virtual ~State() {};
};

