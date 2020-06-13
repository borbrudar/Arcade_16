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
	mr.loadFromFile("res/mario/mario.png");

	//enemy
	en.loadFromFile("res/mario/en1.png");
	en2.loadFromFile("res/mario/en2.png");

	//entities
	tits.loadFromFile("res/mario/entity.png");
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
			//mystery (with coins only for now)
			if (lvl.getPixel(x, y) == Color(75, 0, 255, 255))
				boxes.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::mystery, 1));
			//shiny blocks
			if (lvl.getPixel(x, y) == Color(128, 128, 128, 255))
				boxes.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::shine));
			//coin (as blocks)
			if (lvl.getPixel(x, y) == Color(255, 255, 0, 255))
				coins_.push_back(Box(Vector2f(x * sx + off.x, y * sy + off.y), Vector2f(sx, sy), gr, bSize, block_type::coin));
			//mario
			if (lvl.getPixel(x, y) == Color(255, 0, 0, 255))
				mario.setup(Vector2f(x * sx + off.x, y * sy + off.y - 100), mSize, mr, Vector2f(sx, sy));

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
					e2Size, Vector2f(sx, sy), en2, 1);
			}
		}
	}
}

void Super_Mario::draw(RenderWindow& window)
{
	window.clear(Color(92, 148, 252));
	back.draw(window);

	//boxes and blocks
	for (int i = 0; i < boxes.size(); i++) boxes[i].draw(window);
	for (int i = 0; i < blocks.size(); i++) blocks[i].draw(window);

	for (int i = 0; i < coins_.size(); i++) coins_[i].draw(window);
	//mario
	mario.draw(window);

	//enemies
	for (int i = 0; i < enemies.size(); i++) enemies[i].draw(window);

	//entities
	for (int i = 0; i < entities.size(); i++) {
		entities[i].draw(window);
	}
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
			}
		}
		if (e.type == Event::KeyReleased) {
			switch (e.key.code) {
			case Keyboard::Left: left = 0; break;
			case Keyboard::Right: right = 0; break;
			case Keyboard::Up: up = 0; break;
			}
		}

	}


	//enemies
	bool ecol = 0;
	for (int j = 0; j < enemies.size(); j++) {
		ecol = 0; 
		bool onsc = 0;
		//enemies with boxes
		for (int i = 0; i < boxes.size(); i++) {
			if (boxes[i].box.animation.getGlobalBounds().intersects(enemies[j].anim.animation.getGlobalBounds())) {
				ecol = 1;
				break;
			}
		}
		//with mario
		for (int i = 0; i < mario.mariobox.size(); i++) {
			if (mario.mariobox[i].getGlobalBounds().intersects(enemies[j].anim.animation.getGlobalBounds())) {
				//if touching with bottom
				if(i == 1) enemies[j].alive = 0;
				break;
			}
		}

		//is on screen
		if (enemies[j].anim.animation.getPosition().x < scrWidth) onsc = 1;
		//update
		if (enemies[j].update(ecol, onsc)) {
			enemies.erase(enemies.begin() + j);
			break;
		}
	}
	
	//entities
	for (int i = 0; i < entities.size(); i++) entities[i].update(boxes[i].box.animation.getSize());

	//update box up-down movement
	for(int i = 0; i < boxes.size(); i++) boxes[i].update();
	
	//coin collection
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i].out == 1)
		{
			coins += 1;
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

				//if touching with head and has a coin
				if (j == 0 && boxes[i].entity == 1) {
					entities.push_back(Entity(tits, cSize, 3, boxes[i].box.animation.getPosition(), tSize, 1));
					boxes[i].entity = 0;
				}
			}
		}
	}

	//offset everything if necessary
	if (mario.update(left, right, up, col, type) == 1) {
		offX -= mario.mariosp;
		//blocks
		for (int i = 0; i < boxes.size(); i++) 	boxes[i].off(offX);
		for(int i = 0; i < blocks.size();i++) blocks[i].off(offX);
		for (int i = 0; i < coins_.size(); i++) coins_[i].off(offX);
		//other
		for (int i = 0; i < entities.size(); i++) entities[i].off(offX);
		for (int i = 0; i < enemies.size(); i++) enemies[i].off(offX);
	}

}