#include "Super_Mario.h"

Super_Mario::Super_Mario(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14);

}

void Super_Mario::draw(RenderWindow& window)
{
	back.draw(window);
}

void Super_Mario::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
	}

	if (back.isClicked(mouse, window)) gameState = state::menu;
}
