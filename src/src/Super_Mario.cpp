#include "Super_Mario.h"
#include <math.h>

Super_Mario::Super_Mario(Font& f) : mfont(f)
{
	//songs
	theme.openFromFile("res/mario/audio/theme.wav");
	theme.play(); theme.setLoop(1);
	bl_br.loadFromFile("res/mario/audio/bl_br.wav");
	appear.loadFromFile("res/mario/audio/appear.wav");
	death.loadFromFile("res/mario/audio/death.wav");
	coin.loadFromFile("res/mario/audio/coin.wav");
	jump.loadFromFile("res/mario/audio/jump.wav");
	fire.loadFromFile("res/mario/audio/fire.wav");
	powerup.loadFromFile("res/mario/audio/powerup.wav");

	//scores
	scr.setPosition(20, 10);
	scr.setCharacterSize(24);
	scr.setFillColor(Color::White);
	scr.setFont(f);

	//read from file
	std::string hsc;
	high.open("res/mario/hs.txt");
	std::getline(high, hsc);
	highscore = std::stoi(hsc);
	high.close();

	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 440), 14);

	//boxes
	gr.loadFromFile("res/mario/ground.png");
	br.loadFromFile("res/mario/ground.png");

	//visuals
	cl.loadFromFile("res/mario/cloud.png");
	h1.loadFromFile("res/mario/hill.png");

	//mario
	mr.resize(3);
	mr[0].loadFromFile("res/mario/mario.png");
	mr[1].loadFromFile("res/mario/bigmario.png");
	mr[2].loadFromFile("res/mario/shinymario.png");
	

	//enemy
	en.loadFromFile("res/mario/en1.png");
	en2.loadFromFile("res/mario/en2.png");

	//entities
	tits.loadFromFile("res/mario/entity.png");
	schrooms.loadFromFile("res/mario/schroom.png");

	//level
	lvl.loadFromFile("res/mario/lvl.png");
	float sy = scrHeight / lvl.getSize().y, sx = sy;
	tSize = Vector2f(sx, sy);

	//projectiles
	proj.loadFromFile("res/mario/proj.png");
	xp.loadFromFile("res/mario/exp.png");

	lastX = (scrWidth / sx) + 7;
	loadWorld(0, lastX, 0);
	lastX -= 1;
}

Super_Mario::~Super_Mario()
{
	theme.stop();
	sound.stop();
}

void Super_Mario::draw(RenderWindow& window)
{
	window.clear(Color(92, 148, 252));

	for (int i = 0; i < blocks.size(); i++) blocks[i].draw(window);

	//entities
	for (int i = 0; i < entities.size(); i++) {
		entities[i].draw(window);
	}
	//boxes and blocks
	for (int i = 0; i < boxes.size(); i++) boxes[i].draw(window);

	for (int i = 0; i < coins_.size(); i++) coins_[i].draw(window);
	//mario
	mario.draw(window);

	//enemies
	for (int i = 0; i < enemies.size(); i++) enemies[i].draw(window);

	//projectiles
	for(int i = 0; i < mp.size();i++) mp[i].draw(window);
	for (int i = 0; i < xps.size(); i++) xps[i].draw(window);

	//broklen
	for (int i = 0; i < broken.size(); i++) broken[i].draw(window);

	//draw score and highscore and lives and whatnot
	drawString(20, 10, "Score:", window);
	drawString(40, 40, std::to_string(score), window);

	drawString(130, 10, "Highscore:", window);
	drawString(170, 40, std::to_string(highscore), window);

	drawString(310, 10, "Lives:", window);
	drawString(330, 40, std::to_string(lives), window);

	drawString(420, 10, "Coins:", window);
	drawString(450, 40, std::to_string(coins), window);

	drawString(530, 10, "Time:", window);
	drawString(540, 40, std::to_string(times), window);

	back.draw(window);
}

void Super_Mario::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e, float delta)
{
	//update score
	if (score > highscore) {
		highscore = score;
		//write to file and close it
		high.open("res/mario/hs.txt", std::ios::out | std::ios::trunc);
		high << highscore;
		high.close();
	}

	//button
	if (back.isClicked(mouse, window)) gameState = state::menu;
	//input
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (e.type == Event::KeyPressed) {
			switch (e.key.code) {
			case Keyboard::Left: left = 1; break;
			case Keyboard::Right: right = 1; break;
			case Keyboard::Up: up = 1; break;
			case Keyboard::LShift: sprint = 1; break;
			}
		}
		if (e.type == Event::KeyReleased) {
			switch (e.key.code) {
			case Keyboard::Left: left = 0; break;
			case Keyboard::Right: right = 0; break;
			case Keyboard::Up: up = 0; break;
			case Keyboard::LShift: sprint = 0; break;
			}
		}

	}

	if (mario.alive == 1 && !gameOver) {
		//sounds
		if (!th) { theme.play(); th = 1; }
		if (up && !jp) {
			sound.setBuffer(jump); sound.play();
			jp = 1;
		} else if(mario.groundTouch) jp = 0;

		//update time
		time = clock.getElapsedTime().asSeconds();
		timer += time;
		clock.restart();

		if (timer > delay) {
			timer = 0;
			times -= 1;
		}
		//kill if out of time
		if (times < 0) {
			mario.alive = 0;
			times = 0;
			sound.setBuffer(death); sound.play();
		}

		//projectiles
		//shoot
		if (sprint && mp.size() < 4 && mario.canShoot) {
			Vector2f pos = mario.box.animation.getPosition(), size = mario.box.animation.getSize();
			mp.push_back(MP(Vector2f(pos.x + size.x / 2, pos.y + size.y / 2),
				proj, mpSize, Vector2f(tSize.x / 2, tSize.y / 2)));
			mp.back().oddX = -offX;
			mp.back().offX = offX;

			sound.setBuffer(fire); sound.play();

			if (!mario.prevDir) mp.back().speedx = -mp.back().speedx;
		}
		//update & collision n shit
		for (int i = 0; i < mp.size(); i++) {
			std::vector<int> ptype{ -1,-1,-1,-1 };
			bool alive = 1;
			//with boxes und stuff
			for (int j = 0; j < mp[i].projbox.size(); j++) {
				for (int k = 0; k < boxes.size(); k++) {
					if (boxes[k].box.animation.getGlobalBounds().intersects(mp[i].projbox[j].getGlobalBounds())) {
						ptype[j] = 1;
					}
				}
			}
			//kill them enemies
			for (int j = 0; j < enemies.size(); j++) {
				if (enemies[j].alive && enemies[j].anim.animation.getGlobalBounds().intersects(
					mp[i].box.animation.getGlobalBounds())) {
					//for projectile
					alive = 0;
					//for enemy
					enemies[j].alive = 0;
					score += 100;
				}
			}

			//update
			if (mp[i].update(ptype) || !alive) {
				xps.push_back(Explosion(mp[i].box.animation.getPosition(), xp,
					bSize, tSize));
				xps.back().oddX = -offX;
				xps.back().offX = offX;

				mp.erase(mp.begin() + i);
				break;
			}
		}
		//erase
		for (int i = xps.size() - 1; i >= 0; i--) {
			if (xps[i].update()) xps.erase(xps.begin() + i);
		}

		//enemies
		for (int j = 0; j < enemies.size(); j++) {
			bool onsc = 0;
			std::vector<int> etype{ -1,-1,-1,-1 };
			//enemies with boxes
			for (int k = 0; k < enemies[j].enemybox.size(); k++) {
				for (int i = 0; i < boxes.size(); i++) {
					if (enemies[j].enemybox[k].getGlobalBounds().intersects(
						boxes[i].box.animation.getGlobalBounds())) {
						etype[k] = 1;
						break;
					}
				}

			}

			//with mario
			for (int i = 0; i < mario.mariobox.size(); i++) {
				if (mario.mariobox[i].getGlobalBounds().intersects(enemies[j].anim.animation.getGlobalBounds())) {
					//if touching with bottom
					if (enemies[j].alive == 1 && i == 1) {
						enemies[j].alive = 0;
						score += 100;
					}
					//make big mario smol
					else if (enemies[j].alive == 1 && mario.big == 1) {
						mario.op = 1;
						mario.big = 0;
						mario.shiny = 0;
					}

					//spinny guy
					if (enemies[j].alive == 0 && enemies[j].type == 1) {
						enemies[j].spinning = 1;
						if (mario.mariobox[i].getPosition().x < enemies[j].anim.animation.getPosition().x)
							enemies[j].spin = 1;
						else enemies[j].spin = 2;
					}
					break;
				}
			}

			//with other enemies
			for (int i = 0; i < enemies.size(); i++) {
				//skip if its the same
				if (i == j) continue;
				//check collision
				if (enemies[j].anim.animation.getGlobalBounds().intersects(
					enemies[i].anim.animation.getGlobalBounds()) && enemies[i].spin != 0) {
					enemies[j].alive = 0;
					score += 100;
				}

			}

			//is on screen
			if (enemies[j].anim.animation.getPosition().x < scrWidth) onsc = 1;
			//update
			if (enemies[j].update(etype, onsc, delta)) {
				enemies.erase(enemies.begin() + j);
				break;
			}
			enemies[j].boxUpdate();
		}

		//entities
		for (int i = 0; i < entities.size(); i++) entities[i].boxUpdate();
		for (int i = 0; i < entities.size(); i++) {
			std::vector<int> etype{ -1,-1,-1,-1 };
			//entities with boxes
			for (int k = 0; k < entities[i].entitybox.size(); k++) {
				for (int j = 0; j < boxes.size(); j++) {
					if (entities[i].entitybox[k].getGlobalBounds().intersects(
						boxes[j].box.animation.getGlobalBounds())) {
						etype[k] = 1;
						break;
					}
				}
			}
			entities[i].update(boxes[0].box.animation.getSize(), etype);
		}

		//coin and schroom and flower collection
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i].out == 1 && entities[i].type == 1)
			{
				coins += 1;
				entities.erase(entities.begin() + i);

				sound.setBuffer(coin); sound.play();
				break;
			}
			else if (entities[i].type == 2 &&
				mario.box.animation.getGlobalBounds().intersects(entities[i].anim.animation.getGlobalBounds()) &&
				mario.big == 1 && entities[i].out == 1) {
				mario.shiny = 1;
				entities.erase(entities.begin() + i);

				//sound efx
				sound.setBuffer(powerup); sound.play();

				break;
			}
			else if (entities[i].type == 2 &&
				mario.box.animation.getGlobalBounds().intersects(entities[i].anim.animation.getGlobalBounds()) &&
				entities[i].out == 1) {
				mario.big = 1;
				entities.erase(entities.begin() + i);

				sound.setBuffer(powerup); sound.play();
				break;
			}
		}

		//also coin blocks
		for (int i = 0; i < coins_.size(); i++) {
			if (mario.box.animation.getGlobalBounds().intersects(coins_[i].box.animation.getGlobalBounds()))
			{
				coins += 1;
				coins_.erase(coins_.begin() + i);
				break;
			}
		}
		//update coins and blocks[]
		for (int i = 0; i < coins_.size(); i++) coins_[i].update();
		for (int i = 0; i < blocks.size(); i++) blocks[i].update();

		//update box up-down movement
		for (int i = 0; i < boxes.size(); i++) boxes[i].update();

		//broken stuff
		for (int i = broken.size() - 1; i >= 0; i--) if (broken[i].update()) broken.erase(broken.begin() + i);

		//mario-boxes collision
		bool col = 0;
		std::vector<int> type{ -1,-1,-1,-1 };

		for (int j = 0; j < mario.mariobox.size(); j++) {
			for (int i = 0; i < boxes.size(); i++) {
				if (boxes[i].box.animation.getGlobalBounds().intersects(mario.mariobox[j].getGlobalBounds())) {
					col = 1;
					type[j] = 1;
					//wiggle and stop jumping
					if (j == 0) boxes[i].update(1);

					//if touching with head and has an entity
					if (j == 0 && boxes[i].entity != 0) {
						if (boxes[i].entity == 1) entities.push_back(Entity(tits, cSize,
							3, boxes[i].box.animation.getPosition(), tSize, 1));
						else entities.push_back(Entity(schrooms, sSize,
							0, boxes[i].box.animation.getPosition(), tSize, 2, mario.big));

						boxes[i].entity = 0;

						sound.setBuffer(appear); sound.play();

						//correct offsets
						entities.back().oddX = -offX;
						entities.back().offX = offX;
					}

					//destroy the object if mario big and block is brick
					if (mario.big && j == 0 && boxes[i].type == block_type::brick) {
						boxes.erase(boxes.begin() + i);
						sound.setBuffer(bl_br); sound.play();

						Vector2f mpos = mario.box.animation.getPosition(), msize = mario.box.animation.getSize();
						//broken visuals
						broken.push_back(Broken(gr, tSize, mpos, 1));
						broken.push_back(Broken(gr, tSize, Vector2f(mpos.x, mpos.y + msize.y), 1));

						broken.push_back(Broken(gr, tSize, Vector2f(mpos.x + msize.x, mpos.y)));
						broken.push_back(Broken(gr, tSize, Vector2f(mpos.x + msize.x, mpos.y + msize.y)));
						break;
					}

				}
			}

			for (int i = 0; i < enemies.size(); i++) {
				for (int k = 0; k < mario.mariobox.size(); k++) {
					if (mario.mariobox[k].getGlobalBounds().intersects(enemies[i].anim.animation.getGlobalBounds())) {
						if ((k == 2 || k == 3) && (enemies[i].alive || enemies[i].spin != 0) && !mario.op
							&& !mario.big) {
							mario.alive = 0;

							sound.setBuffer(death); sound.play();
							break;
						}
					}
				}
			}
		}

		if (std::abs(offX / tSize.x) > 180) {
			gameOver = 1;
		}

		//offset everything if necessary
		if (mario.update(left, right, up, col, type, delta, sprint) == 1) {
			offX -= mario.mariosp * delta;

			//update world
			if (sprint) tx += mario.mariosp * mario.sprintsp; else tx += mario.mariosp;
			if (tx > tSize.x) {
				//load world
				tx = 0;
				if (lastX < (lvl.getSize().x - 1)) {
					lastX += 1;
					loadWorld(lastX, 0);
				}
				//delete world
				deleteWorld();
			}

			//blocks
			for (int i = 0; i < boxes.size(); i++) 	boxes[i].off(offX);
			for (int i = 0; i < blocks.size(); i++) blocks[i].off(offX);
			for (int i = 0; i < coins_.size(); i++) coins_[i].off(offX);
			//other
			for (int i = 0; i < entities.size(); i++) entities[i].off(offX);
			for (int i = 0; i < enemies.size(); i++) enemies[i].off(offX);

			for (int i = 0; i < mp.size(); i++) mp[i].off(offX);
			for (int i = 0; i < xps.size(); i++) xps[i].off(offX);

			for (int i = 0; i < broken.size(); i++) broken[i].off(offX);
		}
	}
	else if (gameOver == 1) {
	//start at the beggining
	boxes.clear();	blocks.clear();
	coins_.clear();	enemies.clear();
	entities.clear();	xps.clear();
	mp.clear();

	mario.big = 0; mario.shiny = 0; mario.alive = 1;
	offX = 0;
	time = 400;

	lastX = (scrWidth / tSize.x) + 7;
	loadWorld(0, lastX, 0);
	lastX -= 1;
	gameOver = 0;
	}
	else {
	theme.stop(); th = 0;
	if (mario.update()) {
		//reset the world
		boxes.clear();
		blocks.clear();
		coins_.clear();
		enemies.clear();
		entities.clear();
		xps.clear();
		mp.clear();

		mario.big = 0; mario.shiny = 0; mario.alive = 1;
		score = 0;
		offX = 0;
		times = 400;

		lastX = (scrWidth / tSize.x) + 7;
		loadWorld(0, lastX, 0);
		lastX -= 1;
		}
	}

}

void Super_Mario::loadWorld(int start, int end, bool plus)
{
	int sx = tSize.x, sy = tSize.y;
	if (plus) end = start + 1;

	for (int x = start; x < end; x++) {
		for (int y = 0; y < lvl.getSize().y; y++) {
			Color c = lvl.getPixel(x, y);
			if (!(c == Color(0, 0, 0, 0))) {
				//boxes and mario n other stuff
				{
					//both grounds
					if (c == Color(0, 0, 0, 255)) {
						boxes.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::ground)); continue;
					}
					if (c == Color(0, 0, 0, 254)) {
						blocks.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::ground)); continue;
					}
					//bricks
					if (c == Color(0, 0, 255, 255)) {
						boxes.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::brick)); continue;
					}
					//mystery (coins and muschrooms)
					if (c == Color(75, 0, 255, 255)) {
						boxes.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::mystery, 1)); continue;
					}
					if (c == Color(80, 0, 255, 255)) {
						boxes.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::mystery, 2)); continue;
					}

					//shiny blocks
					if (c == Color(128, 128, 128, 255)) {
						boxes.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::shine)); continue;
					}
					//coin (as blocks)
					if (c == Color(255, 255, 0, 255)) {
						coins_.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::coin)); continue;
					}
					//mario
					if (c == Color(255, 0, 0, 255)) {
						mario.setup(Vector2f(x * sx + off.x,
							y * sy + off.y - 100), mSize, mr, Vector2f(sx, sy)); continue;	}
				}

				//pipes
				{
					//top left
					if (c == Color(0, 255, 255, 255)) {
						boxes.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::ptl)); continue;
					}
					if (c == Color(0, 254, 255, 255)) {
						boxes.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::ptr)); continue;
					}
					if (c == Color(0, 255, 254, 255)) {
						boxes.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::pl)); continue;
					}
					if (c == Color(0, 254, 254, 255)) {
						boxes.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::pr)); continue;
					}
				}

				//enemies
				{
					//normal
					if (c == Color(0, 255, 0, 255)) {
						enemies.push_back(Enemy());
						enemies.back().setup(Vector2f(x * sx + off.x, y * sy + off.y),
							e1Size, Vector2f(sx, sy), en);
						continue;
					}
					//the other thingy
					if (c == Color(0, 128, 0, 255)) {
						enemies.push_back(Enemy());
						enemies.back().setup(Vector2f(x * sx + off.x, y * sy + off.y),
							e2Size, Vector2f(sx, sy * 1.5f), en2, 1);
						continue;
					}

				}

				//visuals 
				{
					//1
					if (c == Color(255, 0, 255, 255)) {
						blocks.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx * 2, sy * 1.5f), cl, c1, block_type::cloud1)); continue;
					}
					if (c == Color(128, 0, 255, 255)) {
						blocks.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx * 2, sy * 1.5f), cl, c1, block_type::grass1)); continue;
					}
					//2
					if (c == Color(200, 0, 200, 255)) {
						blocks.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx * 3, sy * 1.5f), cl, c2, block_type::cloud2)); continue;
					}
					if (c == Color(100, 0, 230, 255)) {
						blocks.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx * 3, sy * 1.5f), cl, c2, block_type::hill1)); continue;
					}
					if (c == Color(130, 0, 200, 255)) {
						blocks.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx * 3, sy * 1.5f), cl, c2, block_type::grass2)); continue;
					}
					//3 
					if (c == Color(150, 0, 150, 255)) {
						blocks.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx * 4, sy * 1.5f), cl, c3, block_type::cloud3)); continue;
					}
					if (c == Color(100, 0, 200, 255)) {
						blocks.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y), Vector2f(sx * 4, sy * 1.5f), cl, c3, block_type::grass3)); continue;
					}
					//4
					if (c == Color(50, 0, 50, 255)) {
						blocks.push_back(Box(Vector2f(x * sx + off.x,
							y * sy + off.y - 1.18 * sy), Vector2f(sx * 5, sy * 2.1875f), h1, c4, block_type::hill2)); continue;
					}
				}
			}
		}
	}
}

void Super_Mario::deleteWorld()
{
	int sx = tSize.x, sy = tSize.y;
	//delete all the fucking unnecessary items
	for (int i = boxes.size() - 1; i >= 0; i--) {
		if (std::floor(boxes[i].box.animation.getPosition().x / sx) == firstX) boxes.erase(boxes.begin() + i);
	}
	for (int i = blocks.size() - 1; i >= 0; i--) {
		if (std::floor(blocks[i].box.animation.getPosition().x / sx) == firstX) blocks.erase(blocks.begin() + i);
	}
	for (int i = coins_.size() - 1; i >= 0; i--) {
		if (std::floor(coins_[i].box.animation.getPosition().x / sx) == firstX) coins_.erase(coins_.begin() + i);
	}
	for (int i = entities.size() - 1; i >= 0; i--) {
		if (std::floor(entities[i].anim.animation.getPosition().x / sx) == firstX) entities.erase(entities.begin() + i);
	}
	for (int i = enemies.size() - 1; i >= 0; i--) {
		if (std::floor(enemies[i].anim.animation.getPosition().x / sx) == firstX) enemies.erase(enemies.begin() + i);
	}

}

void Super_Mario::drawString(int x, int y, std::string string, RenderWindow &window)
{
	std::string text2;
	text2.assign(string);
	scr.setString(text2);
	scr.setFont(mfont);
	scr.setPosition(x,y);
	window.draw(scr);
}
