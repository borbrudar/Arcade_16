#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <random>

using namespace sf;

enum class type {
	block,
	downLeft,
	topRight,
	downRight,
	topLeft
};

class Shield {
	class Shield_Section {
	public:
		void setup(type type, float startX, float startY);
		void draw(RenderWindow& window);
		bool update(Vector2f pos, Vector2f size);
		RectangleShape smallRect;
		int ID = 0;
	private:
		std::array<Vertex, 400> points;
		int size = 20;
	};
public:
	void setup(float startX, float startY);
	void draw(RenderWindow& window);
	std::array<Shield_Section,16> parts;
	int size = 20;
	RectangleShape bigRect;
};