#pragma once
#include <SFML/Graphics.hpp>
#include "Manimation.h"

using namespace sf;

class Entity {
public:
	Entity(Texture &t, Vector2f size, int maxSwap, Vector2f pos, Vector2f tSize, int type, bool marioBig = 0);
	void draw(RenderWindow& window);
	void update(Vector2f until, std::vector<int> etype);
	void off(float offX);
	void boxUpdate();

	std::vector<RectangleShape> entitybox;

	Manimation anim;
	Vector2f pos, oldPos, prevPos;
	int type = 1, wiggling = 1;
	bool out = 0, showHitbox = 0, groundTouch = 1, bigMario = 0;
	float speed = 1.0f, offX = 0.f, oddX = 0.f, gravity = 1.5f;
};