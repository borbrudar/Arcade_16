#include "Pac_Man.h"

Pac_Man::Pac_Man(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14);
}

void Pac_Man::draw(RenderWindow& window)
{
	back.draw(window);
}

void Pac_Man::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
	}

	if (back.isClicked(mouse, window)) gameState = state::menu;
}
