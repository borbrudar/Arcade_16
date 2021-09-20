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

	for (int i = 0; i < parts.size(); i++) parts[i].ID = i;

	bigRect.setSize(Vector2f(size * 4, size * 4));
	bigRect.setPosition(parts[0].smallRect.getPosition());
}

void Shield::draw(RenderWindow& window)
{	
	for (int i = 0; i < parts.size(); i++) parts[i].draw(window);
}


void Shield::Shield_Section::setup(type type, float startX, float startY)
{
	Vertex temp;
	smallRect.setPosition(startX, startY);
	smallRect.setSize(Vector2f(size,size));

	switch (type) {
	case type::block:
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				temp.color = Color::Green;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
		break;
	case type::downLeft:
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				if (x <= y) temp.color = Color::Green; else temp.color = Color::Black;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
		break;
	case type::topRight:
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				if (x >= y) temp.color = Color::Green; else temp.color = Color::Black;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
		break;
	case type::downRight:
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				if (x - 20 > -y) temp.color = Color::Green; else temp.color = Color::Black;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
		break;
	case type::topLeft:
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				if (-x > y - 20) temp.color = Color::Green; else temp.color = Color::Black;
				temp.position = Vector2f(startX + x, startY + y);
				points[y * size + x] = temp;
			}
		}
		break;
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

	bool c = 0;
	Vector2f xp;
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
							xp = Vector2f(myPos.x + x1, myPos.y + y1);
							points[y1 * mySize.x + x1].color = Color::Black;
							c = 1;
							break;
						}
					}
				}
				if (c) break;
			}
			if (c) break;
		}
		if (c) break;
	}

	//explosion thingy
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<float> dist(0, 1);

	Vector2f temp = xp - myPos;
	if (c) {
		for (float x = 0; x < size.x ; x++) {
			for (float y = 0; y < size.y; y++) {
				//base
				if ((temp.y + y) < this->size && (temp.x + x) < this->size) {
					points[(temp.y + y) * this->size + temp.x + x].color = Color::Black;

					//around it to the left/right
					float gl = 7;
					for (int i = 0; i < gl; i++) {
						//right
						if (dist(engine) > i * (1 / gl) && temp.x + x + i < this->size)
							points[(temp.y + y) * this->size + temp.x + x + i].color = Color::Black;
						//left
						if (dist(engine) > i * (1 / gl) && temp.x - i < this->size && temp.x - i >= 0)
							points[(temp.y + y) * this->size + temp.x - i].color = Color::Black;
						//down
						if (dist(engine) > i * (1 / gl) && temp.y + i < this->size)
							points[(temp.y + y) * this->size + temp.x].color = Color::Black;
					}
				}
			}
		}
		
	}

	return c;
}
