#include "Shield.h"

Shield::Shield()
{
	parts.back().setup(0, 320, 500);
}

void Shield::draw(RenderWindow& window)
{	
	for (int i = 0; i < parts.size(); i++) parts[i].draw(window);
}


void Shield::Shield_Section::setup(bool type, float startX, float startY)
{
	if (type == 0) {
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				Vertex temp;
				temp.color = Color::Green;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * 20 + x] = temp;
			}
		}
	}
}

void Shield::Shield_Section::draw(RenderWindow& window)
{
	window.draw(points.data(), points.size(), Points);
}
