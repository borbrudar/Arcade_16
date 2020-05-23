#include "Shield.h"

void Shield::setup(float startX, float startY)
{
	parts[0].setup(type::downRight, startX, startY);
	parts[1].setup(type::block, startX + size, startY);
	parts[2].setup(type::block, startX + 2 * size, startY);
	parts[3].setup(type::downLeft, startX + 3 * size, startY);
	for (int i = 4; i < 12; i++) parts[i].setup(type::block, startX + (i % 4 ) * size,
		startY + (floor(i / 4) )* size);
	parts[12].setup(type::block, startX, startY + 3 * size);
	parts[13].setup(type::topLeft, startX + size, startY + 3 * size);
	parts[14].setup(type::topRight, startX + 2 * size, startY + 3 * size);
	parts[15].setup(type::block, startX + 3 * size, startY + 3 * size);
}

void Shield::draw(RenderWindow& window)
{	
	for (int i = 0; i < parts.size(); i++) parts[i].draw(window);
}


void Shield::Shield_Section::setup(type type, float startX, float startY)
{
	if (type == type::block) {
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				Vertex temp;
				temp.color = Color::Green;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
	}
	if (type == type::downLeft) {
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				Vertex temp;
				if (x <= y) temp.color = Color::Green; else temp.color = Color::Black;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
	}
	if (type == type::topRight) {
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				Vertex temp;
				if (x >= y) temp.color = Color::Green; else temp.color = Color::Black;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
	}
	if (type == type::downRight) {
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				Vertex temp;
				if (x - 20 > -y) temp.color = Color::Green; else temp.color = Color::Black;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
	}
	if (type == type::topLeft) {
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				Vertex temp;
				if (-x > y - 20) temp.color = Color::Green; else temp.color = Color::Black;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
	}
}

void Shield::Shield_Section::draw(RenderWindow& window)
{
	window.draw(points.data(), points.size(), Points);
}
