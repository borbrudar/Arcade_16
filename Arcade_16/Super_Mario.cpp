#include "Super_Mario.h"

Super_Mario::Super_Mario(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14);

	//boxes
	gr.loadFromFile("res/mario/ground.png");
	br.loadFromFile("res/mario/ground.png");

	//mario
	mr.resize(2);
	mr[0].loadFromFile("res/mario/mario.png");
	mr[1].loadFromFile("res/mario/bigmario.png");

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

	for (int x = 0; x < lvl.getSize().x; x++) {
		for (int y = 0; y < lvl.getSize().y; y++) {
			//both grounds
			if (lvl.getPixel(x, y) == Color(0, 0, 0, 255))
				boxes.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx,sy), gr, bSize, block_type::ground));
			if (lvl.getPixel(x, y) == Color(0, 0, 0, 254))
				blocks.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::ground));
			//bricks
			if (lvl.getPixel(x, y) == Color(0, 0, 255, 255))
				boxes.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::brick));
			//mystery (coins and muschrooms)
			if (lvl.getPixel(x, y) == Color(75, 0, 255, 255))
				boxes.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::mystery, 1));
			if (lvl.getPixel(x, y) == Color(80, 0, 255, 255))
				boxes.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::mystery, 2));
			
			//shiny blocks
			if (lvl.getPixel(x, y) == Color(128, 128, 128, 255))
				boxes.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::shine));
			//coin (as blocks)
			if (lvl.getPixel(x, y) == Color(255, 255, 0, 255))
				coins_.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::coin));
			//mario
			if (lvl.getPixel(x, y) == Color(255, 0, 0, 255))
				mario.setup(Vector2f(x * sx + off.x, y * sy + off.y - 100), mSize, mr, Vector2f(sx, sy));

			//pipes
			//top left
			if (lvl.getPixel(x, y) == Color(0, 255, 255, 255))
				boxes.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::ptl));
			if (lvl.getPixel(x, y) == Color(0, 254, 255, 255))
				boxes.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::ptr));
			if (lvl.getPixel(x, y) == Color(0, 255, 254, 255))
				boxes.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::pl));
			if (lvl.getPixel(x, y) == Color(0, 254, 254, 255))
				boxes.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::pr));


			//enemies
			//normal
			if (lvl.getPixel(x, y) == Color(0, 255, 0, 255)) {
				enemies.push_back(Enemy());
				enemies.back().setup(Vector2f(x * sx + off.x, y * sy + off.y),
					e1Size, Vector2f(sx, sy), en);
			}
			//the other thingy
			if (lvl.getPixel(x, y) == Color(0, 128, 0, 255)) {
				enemies.push_back(Enemy());
				enemies.back().setup(Vector2f(x * sx + off.x, y * sy + off.y),
					e2Size, Vector2f(sx, sy * 1.5f), en2, 1);
			}
		}
	}
}

void Super_Mario::draw(RenderWindow& window)
{
	window.clear(Color(92, 148, 252));
	back.draw(window);

	//entities
	for (int i = 0; i < entities.size(); i++) {
		entities[i].draw(window);
	}
	//boxes and blocks
	for (int i = 0; i < boxes.size(); i++) boxes[i].draw(window);
	for (int i = 0; i < blocks.size(); i++) blocks[i].draw(window);

	for (int i = 0; i < coins_.size(); i++) coins_[i].draw(window);
	//mario
	mario.draw(window);

	//enemies
	for (int i = 0; i < enemies.size(); i++) enemies[i].draw(window);

}

void Super_Mario::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{

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
				if(enemies[j].alive == 1 && i == 1) enemies[j].alive = 0;
				//make big mario smol
				else if (mario.big == 1) mario.big = 0;
					

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
			}

		}

		//is on screen
		if (enemies[j].anim.animation.getPosition().x < scrWidth) onsc = 1;
		//update
		if (enemies[j].update(etype, onsc)) {
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

	//coin and schroom collection
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i].out == 1 && entities[i].type == 1)
		{
			coins += 1;
			entities.erase(entities.begin() + i);
			break;
		}
		else if (entities[i].type == 2 && 
			mario.box.animation.getGlobalBounds().intersects(entities[i].anim.animation.getGlobalBounds())) {
			mario.big = 1;
			entities.erase(entities.begin() + i);
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
	for(int i = 0; i < boxes.size(); i++) boxes[i].update();

	mario.boxUpdate();
	//mario-boxes collision
	bool col = 0;
	std::vector<int> type{ -1,-1,-1,-1 };
	for (int i = 0; i < boxes.size(); i++) {
		for (int j = 0; j < mario.mariobox.size(); j++) {
			if (boxes[i].box.animation.getGlobalBounds().intersects(mario.mariobox[j].getGlobalBounds())) {
				col = 1;
				type[j] = j;
				//wiggle and stop jumping
				if (j == 0) boxes[i].update(1);

				//if touching with head and has an entity
				if (j == 0 && boxes[i].entity != 0) {
					if(boxes[i].entity == 1) entities.push_back(Entity(tits, cSize,
						3, boxes[i].box.animation.getPosition(), tSize, 1));
					else entities.push_back(Entity(schrooms, sSize,
						0, boxes[i].box.animation.getPosition(), tSize, 2, mario.big));

					boxes[i].entity = 0;
					//correct offsets
					entities.back().oddX = -offX;
					entities.back().offX = offX;
				}
			}
		}
	}

	//offset everything if necessary
	if (mario.update(left, right, up, col, type, sprint) == 1) {
		if (sprint) offX -= mario.mariosp * mario.sprintsp;  else offX -= mario.mariosp;
		//blocks
		for (int i = 0; i < boxes.size(); i++) 	boxes[i].off(offX);
		for(int i = 0; i < blocks.size();i++) blocks[i].off(offX);
		for (int i = 0; i < coins_.size(); i++) coins_[i].off(offX);
		//other
		for (int i = 0; i < entities.size(); i++) entities[i].off(offX);
		for (int i = 0; i < enemies.size(); i++) enemies[i].off(offX);
	}

}