#include "Simon.h"

Simon::Simon(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14);

	//boxes
	boxes.resize(4);
	for (int i = 0; i < boxes.size(); i++) {
		boxes[i].setSize(bSize);
		boxes[i].setPosition(Vector2f(start.x + bSize.x * (i % 2), start.y + bSize.y * std::floor(i / 2)));
	}

	boxes[0].setFillColor(Color::Red);
	boxes[1].setFillColor(Color::Green);
	boxes[2].setFillColor(Color::Blue);
	boxes[3].setFillColor(Color::Yellow);
}

void Simon::draw(RenderWindow& window)
{
	back.draw(window);

	for (int i = 0; i < boxes.size(); i++) window.draw(boxes[i]);
}

void Simon::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
	}

	if (back.isClicked(mouse, window)) gameState = state::menu;
}
