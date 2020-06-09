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
		boxes[i].box.setSize(bSize);
		boxes[i].box.setPosition(Vector2f(start.x + bSize.x * (i % 2), start.y + bSize.y * std::floor(i / 2)));
	}

	boxes[0].setup(Color::Red);
	boxes[1].setup(Color::Green);
	boxes[2].setup(Color::Blue);
	boxes[3].setup(Color::Yellow);
}

void Simon::draw(RenderWindow& window)
{
	back.draw(window);

	for (int i = 0; i < boxes.size(); i++) window.draw(boxes[i].box);
}

void Simon::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (Mouse::isButtonPressed(Mouse::Button::Left)) {
			//check for touch
			for (int i = 0; i < boxes.size(); i++) {
				if (boxes[i].box.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
					//set the colors straight
					float r = 0, g = 0, b = 0;
					if (boxes[i].oldr * 1.5 > 255) r = 255; else
						r = boxes[i].oldr * 1.25;
					if (boxes[i].oldg * 1.5 > 255) g = 255; else
						g = boxes[i].oldg * 1.25;
					if (boxes[i].oldb * 1.5 > 255) b = 255; else
						b = boxes[i].oldb * 1.25;

					boxes[i].box.setFillColor(Color(r,g,b));
				}
			}
		}
	}

	if (back.isClicked(mouse, window)) gameState = state::menu;

	//slowly subtract colors
	for (int i = 0; i < boxes.size(); i++) boxes[i].update();
}

void Box::setup(Color col)
{
	oldr = col.r * k;
	oldg = col.g * k;
	oldb = col.b * k;

	box.setFillColor(Color(oldr, oldg, oldb));
}

void Box::update()
{
	//slowly subtract colors
	if (box.getFillColor().r > oldr) box.setFillColor(Color(box.getFillColor().r - 1,
		box.getFillColor().g, box.getFillColor().b));

	if (box.getFillColor().g > oldg) box.setFillColor(Color(box.getFillColor().r,
		box.getFillColor().g - 1, box.getFillColor().b));
	
	if (box.getFillColor().b > oldb) box.setFillColor(Color(box.getFillColor().r,
		box.getFillColor().g, box.getFillColor().b - 1));
}
