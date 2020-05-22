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

	death.setup("res/space/death.png", dSize);
	
	//projectiles
	proj.loadFromFile("res/space/projectile.png");

	alienI.resize(invdM);
	for (int i = 0; i < alienI.size(); i++) alienI[i] = invdN - 1;

	//explosion
	xp.loadFromFile("res/space/explosion.png");
	xp2.loadFromFile("res/space/explosion2.png");

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

	si.setPosition(300, 500);
	si.setSize(Vector2f(5, 5));
	si.setFillColor(Color::Green);
}

void Space_Invaders::draw(RenderWindow& window)
{
	if (!transition) window.draw(cannon);
	else if(transition)	death.draw(window);

	back.draw(window);

	for (int x = 0; x < invdM; x++) {
		for (int y = 0; y < invdN; y++) invaders[x][y].draw(window);
	}

	//all bullets
	for (int i = 0; i < bullets.size(); i++) bullets[i].draw(window);
	for (int i = 0; i < alienBullets.size(); i++) alienBullets[i].draw(window);

	//xp
	for (int i = 0; i < explosions.size(); i++) window.draw(explosions[i]);

	window.draw(si);
}

void Space_Invaders::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	//clock
	time = bClock.getElapsedTime().asSeconds();
	timer += time;
	bClock.restart();

	cannonx = 0;
	if (back.isClicked(mouse, window)) gameState = state::menu;

	if (!gameOver) {
		if (!transition) {
			dtimer = 0;
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

			//cannon
			if (left) cannonx = speedL;
			if (right) cannonx = speedR;
			if (shoot && (timer > delay)) {
				//bullet position
				bullets.push_back(Projectile(proj,
					Vector2f((float)cannon.getPosition().x + (float)(cSize.x / 2.f - 0.5f) * space_scale,
						cannon.getPosition().y - bSize.y * space_scale)));
				bullets.back().speedy = -2.5f;
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

			//alien bullets
			std::random_device rd;
			std::default_random_engine engine(rd());
			std::uniform_int_distribution<int> dist(0, invdM - 1);


			atime = aClock.getElapsedTime().asSeconds();
			atimer += atime;
			aClock.restart();
			if (atimer > adelay) {
				atimer = 0;
				int rand = 0;
				bool bul = 0, chosen = 0;
				for (int i = 0; i < alienI.size(); i++) if (alienI[i] >= 0) { bul = 1; break; }
				while (bul) {
					rand = dist(engine);
					if (alienI[rand] >= 0) { bul = 0; chosen = 1; }
				}

				if (chosen) {
					int x = invaders[rand][alienI[rand]].animation.animation.getPosition().x;
					int y = invaders[rand][alienI[rand]].animation.animation.getPosition().y;
					Vector2f size = bSize;
					alienBullets.push_back(Projectile(proj, Vector2f(x + size.x / 2, y + size.y + 1)));
					alienBullets.back().speedy = -alienBullets.back().speedy;
				}
			}

			//bullets
			//collision with invaders
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

							alienI[x]--;
							temp = 1;
							break;
						}
					}
					if (temp) break;
				}
			}

			//collision with other bullets
			for (int i = 0; i < bullets.size(); i++) {
				bool temp = 0;
				for (int j = 0; j < alienBullets.size(); j++) {
					if (bullets[i].projectile.getGlobalBounds().intersects(alienBullets[j].projectile.getGlobalBounds())) {
						//explosion
						explosions.push_back(Sprite(xp2));
						explosions.back().setPosition(alienBullets[j].projectile.getPosition());
						explosions.back().setScale(space_scale, space_scale);
						xptimer.push_back(0);
						//erase both bullets
						bullets.erase(bullets.begin() + i);
						alienBullets.erase(alienBullets.begin() + j);
						break;
					}
				}
				if (temp) break;
			}

			//collision with cannon
			for (int i = 0; i < alienBullets.size(); i++) {
				if (cannon.getGlobalBounds().intersects(alienBullets[i].projectile.getGlobalBounds())) {
					bullets.clear();
					alienBullets.clear();
					transition = 1;

					deathC.restart();
					death.animation.setScale(space_scale, space_scale);
					death.animation.setPosition(cannon.getPosition());
					left = 0; right = 0; shoot = 0;

					lives--;
					if (lives <= 0) gameOver = 1;
				}
			}

			//border bullets
			for (int i = 0; i < bullets.size(); i++) {
				if (bullets[i].pos.y < -30 || bullets[i].pos.y > scrHeight) {
					bullets.erase(bullets.begin() + i);
				}
			}


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
					for (int y = 0; y < invdN; y++) if (invaders[x][y].alive) invaders[x][y].speedx = sped;
				}
			}

			//update positions
			for (int x = 0; x < invdM; x++) {
				for (int y = 0; y < invdN; y++) invaders[x][y].animation.animation.move(invaders[x][y].speedx, 0);
			}
		}
		else {
			while (window.pollEvent(e)) {
				if (e.type == Event::Closed) window.close();
			}

			dtime = deathC.getElapsedTime().asSeconds();
			dtimer += dtime;
			deathC.restart();

			if (dtimer > ddelay) transition = 0;
		}
	}
    else {
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::R) {
					gameOver = 0;
					lives = 3;
				}
			}
		}
    }
}
