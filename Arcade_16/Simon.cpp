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

	for(int i = 0; i < boxes.size();i++) boxes[i].setFillColor(Color(boxes[i].getFillColor().r * 0.7,
		boxes[i].getFillColor().g * 0.7, boxes[i].getFillColor().b * 0.7));
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
		if (Mouse::isButtonPressed(Mouse::Button::Left)) {
			//check for touch
			for (int i = 0; i < boxes.size(); i++) {
				if (boxes[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
					//set the colors straight
					float r = 0, g = 0, b = 0;
					if (boxes[i].getFillColor().r * 1.5 > 255) r = 255; else
						r = boxes[i].getFillColor().r * 1.25;
					if (boxes[i].getFillColor().g * 1.5 > 255) g = 255; else
						g = boxes[i].getFillColor().g * 1.25;
					if (boxes[i].getFillColor().b * 1.5 > 255) b = 255; else
						b = boxes[i].getFillColor().b * 1.25;

					boxes[i].setFillColor(Color(r,g,b));
				}
			}
		}
	}

	if (back.isClicked(mouse, window)) gameState = state::menu;

	
}
