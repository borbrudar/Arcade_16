#include "Space_Invaders.h"
#include "Space_Invaders.h"

Space_Invaders::Space_Invaders(Font& f)
{
	//scores
	scr.setPosition(20, 10);
	scr.setCharacterSize(26);
	scr.setFillColor(Color::White);
	scr.setFont(f);

	//read from file
	std::string hsc;
	high.open("res/space/hs.txt");
	std::getline(high, hsc);
	highscore = std::stoi(hsc);
	high.close();

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
	for (int x = 0; x < invdM; x++) {
		invaders[x].resize(invdN);
		for (int y = 0; y < invdN; y++) {
			if(y == 0 || y == 1) invaders[x][y].setup("res/space/alien2.png", size2, Vector2f(x,y), spacing2, off);
			if(y == 2 || y == 3) invaders[x][y].setup("res/space/alien1.png", size1, Vector2f(x, y), spacing1, off);
			if(y == 4 || y == 5) invaders[x][y].setup("res/space/alien3.png", size3, Vector2f(x, y), spacing3, off);
		}
	}

	//shield is 80 wide, 5 spaces of 64 = 640 (scrWidth)
	shields.resize(4);
	shields[0].setup(64, 350);
	shields[1].setup(208, 350);
	shields[2].setup(352, 350);
	shields[3].setup(492, 350);

	//lives
	dlife.setTexture(c);
}

void Space_Invaders::draw(RenderWindow& window)
{
	window.clear(Color::Black);

	if (!transition) window.draw(cannon);
	else if(transition)	death.draw(window);

	back.draw(window);
	
	//shields
	for (int i = 0; i < shields.size(); i++) shields[i].draw(window);

	//invaders
	for (int x = 0; x < invdM; x++) {
		for (int y = 0; y < invdN; y++) invaders[x][y].draw(window);
	}

	//all bullets
	for (int i = 0; i < bullets.size(); i++) bullets[i].draw(window);
	
	for (int i = 0; i < alienBullets.size(); i++) alienBullets[i].draw(window);

	//xp
	for (int i = 0; i < explosions.size(); i++) window.draw(explosions[i]);

	//draw score and highscore
	std::string text2;
	text2.assign("Score: ");
	text2 = text2 + std::to_string(score);
	scr.setString(text2);
	scr.setPosition(20, 10);
	window.draw(scr);

	text2.assign("Highscore: ");
	text2 = text2 + std::to_string(highscore);
	scr.setString(text2);
	scr.setPosition(200, 10);
	window.draw(scr);

	//lives indicator
	for (int i = 0; i < lives; i++) {
		dlife.setScale(space_scale, space_scale);
		dlife.setPosition((400 + i * space_scale * c.getSize().x + i * 10), 10);
		window.draw(dlife);
	}
}

void Space_Invaders::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	//update score
	if (score > highscore) {
		highscore = score;
		//write to file and close it
		high.open("res/space/hs.txt", std::ios::out | std::ios::trunc);
		high << highscore;
		high.close();
	}

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
				bullets.back().speedy = cannonshoot;
				//clock reset
				timer = 0.f;
			}

			//cAnOn
			cannon.move(cannonx, 0);
			if (cannon.getPosition().x < 0) cannon.setPosition(0, cannon.getPosition().y);
			if (cannon.getPosition().x + cSize.x * space_scale > scrWidth) cannon.setPosition(scrWidth - cSize.x * space_scale, cannon.getPosition().y);

			//explosion
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
					float x = invaders[rand][alienI[rand]].animation.animation.getPosition().x;
					float y = invaders[rand][alienI[rand]].animation.animation.getPosition().y;
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

							//update score
							if (y == 5 || y == 4) score += 10;
							else if (y == 3 || y == 2) score += 20;
							else score += 30;
							
							//check if there are any left
							inThere = 0;
							bool c = 0;
							for (int in = 0; in < invaders.size(); in++) {
								for (int in2 = 0; in2 < invaders[in].size(); in2++) {
									if (invaders[in][in2].animation.animation.getPosition().y > 0) {
										inThere = 1;
										c = 1;
										break;
									}
								}
								if (c) break;
							}

							//other
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

			//collision with the shield
			for (int i = 0; i < alienBullets.size(); i++) {
				bool t = 0;
				for (int j = 0; j < shields.size(); j++) {
					if (alienBullets[i].projectile.getGlobalBounds().intersects(
						shields[j].bigRect.getGlobalBounds())) {

						//small part of the shield
						for (int k = 0; k < shields[j].parts.size(); k++) {
							if (shields[j].parts[k].smallRect.getGlobalBounds().intersects(
								alienBullets[i].projectile.getGlobalBounds())) {

								//funct to delete the shield
								Vector2f tpos = alienBullets[i].projectile.getPosition();
								if (shields[j].parts[k].update(tpos, Vector2f(bSize.x * space_scale, bSize.y * space_scale))) {
									//misc
									alienBullets.erase(alienBullets.begin() + i);
									t = 1;
									break;
								}
							}
						}
					}
					if (t) break;
				}
				if (t) break;
			}

			for (int i = 0; i < bullets.size(); i++) {
				bool t = 0;
				for (int j = 0; j < shields.size(); j++) {
					if (bullets[i].projectile.getGlobalBounds().intersects(
						shields[j].bigRect.getGlobalBounds())) {

						//small part of the shield
						for (int k = 0; k < shields[j].parts.size(); k++) {
							if (shields[j].parts[k].smallRect.getGlobalBounds().intersects(
								bullets[i].projectile.getGlobalBounds())) {

								//funct to delete the shield
								Vector2f tpos = bullets[i].projectile.getPosition();
								if (shields[j].parts[k].update(tpos, Vector2f(bSize.x * space_scale, bSize.y * space_scale))) {
									//misc
									bullets.erase(bullets.begin() + i);
									t = 1;
									break;
								}
							}
						}
					}
					if (t) break;
				}
				if (t) break;
			}

			//border bullets
			for (int i = 0; i < bullets.size(); i++) {
				if (bullets[i].pos.y < -30 || bullets[i].pos.y > scrHeight) {
					bullets.erase(bullets.begin() + i);
				}
			}

			//border detecion for invaders
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
		//transition period
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
	//restart the game
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::R) {
					gameOver = 0;
					lives = 3;
					score = 0;
					//shields
					shields.clear();
					shields.resize(4);
					shields[0].setup(64, 350);
					shields[1].setup(208, 350);
					shields[2].setup(352, 350);
					shields[3].setup(492, 350);
					//invaders
					for(int i = 0; i < alienI.size();i++) alienI[i] = invdN - 1;
					invaders.clear();
					invaders.resize(invdM);
					for (int x = 0; x < invdM; x++) {
						invaders[x].resize(invdN);
						for (int y = 0; y < invdN; y++) {
							if (y == 0 || y == 1) invaders[x][y].setup("res/space/alien2.png", size2, Vector2f(x, y), spacing2, off);
							if (y == 2 || y == 3) invaders[x][y].setup("res/space/alien1.png", size1, Vector2f(x, y), spacing1, off);
							if (y == 4 || y == 5) invaders[x][y].setup("res/space/alien3.png", size3, Vector2f(x, y), spacing3, off);
						}
					}
				}
			}
		}
    }

	//new wave of invaders
	if (!inThere) {
		//invaders
		for (int i = 0; i < alienI.size(); i++) alienI[i] = invdN - 1;
		inThere = 1;
		invaders.clear();
		invaders.resize(invdM);
		for (int x = 0; x < invdM; x++) {
			invaders[x].resize(invdN);
			for (int y = 0; y < invdN; y++) {
				if (y == 0 || y == 1) invaders[x][y].setup("res/space/alien2.png", size2, Vector2f(x, y), spacing2, off);
				if (y == 2 || y == 3) invaders[x][y].setup("res/space/alien1.png", size1, Vector2f(x, y), spacing1, off);
				if (y == 4 || y == 5) invaders[x][y].setup("res/space/alien3.png", size3, Vector2f(x, y), spacing3, off);
			}
		}
	}
}
