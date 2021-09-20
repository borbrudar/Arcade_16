#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Button {
public:
	Button() = default;
//functions
	void setup(Font &f, std::string string, Color c, Vector2f size, Vector2f pos, int sizey);
	void draw(RenderWindow &window);
	bool isClicked(Mouse &mouse, RenderWindow& window);
	void outline(float size, Color c = Color::Black);
//variables
	RectangleShape button;
	Text text;
};