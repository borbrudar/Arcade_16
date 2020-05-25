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

	bigRect.setSize(Vector2f(size * 4, size * 4));
	bigRect.setPosition(parts[0].smallRect.getPosition());
}

void Shield::draw(RenderWindow& window)
{	
	for (int i = 0; i < parts.size(); i++) parts[i].draw(window);
}


void Shield::Shield_Section::setup(type type, float startX, float startY)
{
	smallRect.setPosition(startX, startY);
	smallRect.setSize(Vector2f(size,size));
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
	else if (type == type::downLeft) {
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				Vertex temp;
				if (x <= y) temp.color = Color::Green; else temp.color = Color::Black;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
	}
	else if (type == type::topRight) {
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				Vertex temp;
				if (x >= y) temp.color = Color::Green; else temp.color = Color::Black;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
	}
	else if (type == type::downRight) {
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				Vertex temp;
				if (x - 20 > -y) temp.color = Color::Green; else temp.color = Color::Black;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
	}
	else if (type == type::topLeft) {
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

bool Shield::Shield_Section::update(Vector2f pos, Vector2f size)
{
	Vector2f myPos = smallRect.getPosition();
	Vector2f mySize = smallRect.getSize();

	int count = 0;
	//bullet loop
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			//shield loop
			for (int x1 = 0; x1 < mySize.x; x1++) {
				for (int y1 = 0; y1 < mySize.y; y1++) {
					//check for pixel-pixel collision
					if (((int)myPos.x + (int)x1) == ((int)pos.x + (int)x) &&
						((int)myPos.y + (int)y1) == ((int)pos.y + (int)y)) {
						//only if it's not already black
						if (points[y1 * mySize.x + x1].color != Color::Black) {
							//update
							points[y1 * mySize.x + x1].color = Color::Black;
							count++;

						}
					}
				}
			}
		}
	}


	//return
	if (count > 1) return 1; else return 0;
}
