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

	//explosion
	xp.loadFromFile("res/space/explosion.png");

	//invaders
	invaders.resize(invdM);
	for (float x = 0; x < invdM; x++) {
		invaders[x].resize(invdN);
		for (float y = 0; y < invdN; y++) {
			if(y == 0 || y == 1) invaders[x][y].setup("res/space/alien2.png", size2, Vector2f(x,y), spacing2, off);
			if(y == 2 || y == 3) invaders[x][y].setup("res/space/alien1.png", size1, Vector2f(x, y), spacing1, off);
			if(y == 4 || y == 5) invaders[x][y].setup("res/space/alien3.png", size3, Vector2f(x, y), spacing3, off);
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

	//xp
	for (int i = 0; i < explosions.size(); i++) window.draw(explosions[i]);
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

	//cAnOn
	cannon.move(cannonx, 0);
	if (cannon.getPosition().x < 0) cannon.setPosition(0, cannon.getPosition().y);
	if (cannon.getPosition().x + cSize.x * space_scale > scrWidth) cannon.setPosition(scrWidth - cSize.x * space_scale, cannon.getPosition().y);
	
	//xp
	time = xpClock.getElapsedTime().asSeconds();
	xpClock.restart();
	for (int i = 0; i < explosions.size(); i++) {
		xptimer[i] += time;
		if (xptimer[i] > xpdelay) {
			xptimer.erase(xptimer.begin() + i);
			explosions.erase(explosions.begin() + i);
		}
	}

	//bullet
	for (int i = 0; i < bullets.size(); i++) {
		bool temp = 0;
		for (int x = 0; x < invdM; x++) {
			for (int y = 0; y < invdN; y++) {
				//detect collision
				if (bullets[i].projectile.getGlobalBounds().intersects(invaders[x][y].animation.animation.getGlobalBounds())) {
					//explosion update
					explosions.push_back(Sprite(xp));
					explosions.back().setPosition(invaders[x][y].animation.animation.getPosition());
					explosions.back().setScale(space_scale, space_scale);
					xptimer.push_back(0);
					//bullets and invaders update
					bullets.erase(bullets.begin() + i);
					invaders[x][y].speedx = 0;
					invaders[x][y].animation.animation.setPosition(50, -50);
					invaders[x][y].alive = 0;
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
	float sped;
	bool temp = 0;
	for (int x = 0; x < invdM; x++) {
		for (int y = 0; y < invdN; y++) {
			if (invaders[x][y].animation.animation.getPosition().x < 0 ||
				invaders[x][y].animation.animation.getPosition().x + invaders[x][y].animation.size.x * space_scale > scrWidth) {
				sped = -invaders[x][y].speedx;
				temp = 1;
				break;
			}
		}
		if (temp) break;
	}

	//deletion
	if (temp) {
		for (int x = 0; x < invdM; x++) {
			for (int y = 0; y < invdN; y++) if(invaders[x][y].alive) invaders[x][y].speedx = sped;
		}
	}

	//update positions
	for (int x = 0; x < invdM; x++) {
		for (int y = 0; y < invdN; y++) invaders[x][y].animation.animation.move(invaders[x][y].speedx, 0);
	}
}
