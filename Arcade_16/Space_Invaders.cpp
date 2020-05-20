#include "Space_Invaders.h"

Space_Invaders::Space_Invaders(Font& f)
{
	//back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(40, 25), Vector2f(580, 2), 12);

	//canon 
	c.loadFromFile("res/space/cannon.png");
	cannon.setTexture(c);
	cannon.setPosition(scrWidth / 2, scrHeight / 12 * 11);
	cannon.setScale(space_scale, space_scale);

	//projectile
	proj.loadFromFile("res/space/projectile.png");

	//invaders
	invaders.resize(invdM);
	for (int x = 0; x < invdM; x++) {
		invaders[x].resize(invdN);
		for (int y = 0; y < invdN; y++) {
			//dirty solution
			if(y == 0 || y == 1) invaders[x][y].setup("res/space/alien2.png", size2);
			if(y == 2 || y == 3) invaders[x][y].setup("res/space/alien1.png", size1);
			if(y == 4 || y == 5) invaders[x][y].setup("res/space/alien3.png", size3);

			invaders[x][y].animation.setScale(Vector2f(space_scale, space_scale));
			if(y == 0 || y == 1) invaders[x][y].animation.setPosition(x * (size2.x * space_scale + spacing2.x) + offsetX, 
				y * (size2.y * space_scale + spacing2.y) + offsetY);
			if (y == 2 || y == 3) invaders[x][y].animation.setPosition(x * (size1.x * space_scale + spacing1.x) + offsetX,
				y * (size1.y * space_scale + spacing1.y) + offsetY);
			if(y == 4 || y == 5) invaders[x][y].animation.setPosition(x * (size3.x * space_scale + spacing3.x) + offsetX,
				y * (size3.y * space_scale + spacing3.y) + offsetY);
		}
	}
}

void Space_Invaders::draw(RenderWindow& window)
{
	back.draw(window);

	window.draw(cannon);

	for (int x = 0; x < invdM; x++) {
		for (int y = 0; y < invdN; y++) invaders[x][y].draw(window);
	}

	for (int i = 0; i < bullets.size(); i++) bullets[i].draw(window);

}

void Space_Invaders::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	//clock
	time = bClock.getElapsedTime().asSeconds();
	timer += time;
	bClock.restart();

	cannonx = 0;

	//click clock
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (e.type == Event::KeyPressed) {
			if (e.key.code == Keyboard::Left) left = 1; 
			if (e.key.code == Keyboard::Right) right = 1;
			if (e.key.code == Keyboard::Space) shoot = 1; 
		}
		if (e.type == Event::KeyReleased) {
			if (e.key.code == Keyboard::Left) left = 0;
			if (e.key.code == Keyboard::Right) right = 0;
			if (e.key.code == Keyboard::Space) shoot = 0;
		}
	}
	if (back.isClicked(mouse, window)) gameState = state::menu;

	//cannon
	if(left) cannonx = speedL;
	if (right) cannonx = speedR;
	if(shoot && (timer > delay)) {
		//bullet position
		bullets.push_back(Projectile(proj,
			Vector2f((float)cannon.getPosition().x + (float) (cSize.x / 2.f - 0.5f) * space_scale,
				cannon.getPosition().y - bSize.y * space_scale)));
		//clock reset
		timer = 0.f;
	}

	
	//bullet
	for (int i = 0; i < bullets.size(); i++) {
		bool temp = 0;
		for (int x = 0; x < invdM; x++) {
			for (int y = 0; y < invdN; y++) {
				if (bullets[i].projectile.getGlobalBounds().intersects(invaders[x][y].animation.getGlobalBounds())) {
					bullets.erase(bullets.begin() + i);
					temp = 1;
					break;
				}
			}
			if (temp) break;
		}
	}
	
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].pos.y < -30) {
			bullets.erase(bullets.begin() + i);
		}
	}
	//int c = 0; -c * (sizeX + spacing)
	//border detecion
	bool temp = 0;
	for (int x = 0; x < invdM; x++) {
		for (int y = 0; y < invdN; y++) {
			if (invaders[x][y].animation.getPosition().x < 0 ||
				invaders[x][y].animation.getPosition().x + invaders[x][y].size.x * space_scale > scrWidth) {
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

	//cannon movement
	cannon.move(cannonx,0);
	if (cannon.getPosition().x < 0) cannon.setPosition(0, cannon.getPosition().y);
	if(cannon.getPosition().x + cSize.x * space_scale > scrWidth) cannon.setPosition(scrWidth - cSize.x * space_scale, cannon.getPosition().y);
}
