#include "Button.h"

void Button::setup(Font& f, std::string string, Color c, Vector2f size, Vector2f pos, int sizey)
{
	button.setFillColor(c);
	button.setSize(size);
	button.setPosition(pos);

	text.setFillColor(Color::Black);
	text.setFont(f);
	text.setCharacterSize(sizey);
	text.setString(string);

	text.setPosition(Vector2f(pos.x + size.x/32, pos.y + size.y/4));
}

void Button::draw(RenderWindow &window)
{
	window.draw(button);
	window.draw(text);
}

bool Button::isClicked(Mouse &mouse, RenderWindow& window)
{
	Vector2i temp = mouse.getPosition(window);
	Vector2f pos = button.getPosition();
	Vector2f size = button.getSize();

	if ((temp.x > pos.x) && (temp.x < (pos.x + size.x)) && 
		(temp.y > pos.y) && (temp.y < (pos.y + size.y)) && mouse.isButtonPressed(Mouse::Left)) return true;

	else return false;
}

void Button::outline(float size, Color c)
{
	button.setOutlineThickness(size);
	button.setOutlineColor(c);
}


