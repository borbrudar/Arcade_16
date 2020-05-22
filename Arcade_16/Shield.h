#pragma once
#include <SFML/Graphics.hpp>
#include <array>

using namespace sf;

class Shield {
	class Shield_Section {
	public:
		void setup(bool type, float startX, float startY);
		void draw(RenderWindow& window);
	private:
		std::array<Vertex, 400> points;
	};
public:
	Shield();
	void draw(RenderWindow& window);
	std::array<Shield_Section,1> parts;
};