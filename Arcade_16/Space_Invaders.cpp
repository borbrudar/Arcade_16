#include "Space_Invaders.h"

Space_Invaders::Space_Invaders(Font& f)
{
	//back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(40, 25), Vector2f(580, 2), 12);

	//invaders
	invaders.resize(invdM);
	for (int x = 0; x < invdM; x++) {
		invaders[x].resize(invdN);
		for (int y = 0; y < invdN; y++) {
			//dirty solutions
			if(y == 0 || y == 1) invaders[x][y].setup("res/space/alien2.png", size2);
			if(y == 2 || y == 3) invaders[x][y].setup("res/space/alien1.png", size1);

			invaders[x][y].animation.setScale(Vector2f(space_scale, space_scale));
			if(y == 0 || y == 1) invaders[x][y].animation.setPosition(x * (size2.x * space_scale + spacing2.x) + offsetX, 
				y * (size2.y * space_scale + spacing2.y) + offsetY);
			if (y == 2 || y == 3) invaders[x][y].animation.setPosition(x * (size1.x * space_scale + spacing1.x) + offsetX,
				y * (size1.y * space_scale + spacing1.y) + offsetY);
		}
	}
}

void Space_Invaders::draw(RenderWindow& window)
{
	back.draw(window);

	for (int x = 0; x < invdM; x++) {
		for (int y = 0; y < invdN; y++) invaders[x][y].draw(window);
	}

}

void Space_Invaders::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
	}
	if (back.isClicked(mouse, window)) gameState = state::menu;

	//int c = 0; -c * (sizeX + spacing)
	//border detecion
	bool temp = 0;
	for (int x = 0; x < invdM; x++) {
		for (int y = 0; y < invdN; y++) {
			if (invaders[x][y].animation.getPosition().x < 0 ||
				invaders[x][y].animation.getPosition().x + invaders[x][y].size.x > scrWidth) {
				speedx = -speedx;
				temp = 1;
				break;
			}
		}
		if (temp) break;
	}


	//update positions
	for (int x = 0; x < invdM; x++) {
		for (int y = 0; y < invdN; y++) invaders[x][y].animation.move(speedx, 0);
	}
}
