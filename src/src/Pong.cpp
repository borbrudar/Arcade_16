#include "Pong.h"

Pong::Pong(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::White, Vector2f(40, 30), Vector2f(scrWidth / 2 - 20, 10), 14);

	//sound
	col.loadFromFile("res/pong/col.wav");
	death.loadFromFile("res/pong/death.wav");

	//player setup
	player.setFillColor(Color::White);
	player.setSize(Vector2f(12, 40));
	player.setPosition(50, scrHeight / 2);

	//ai
	ai.setFillColor(Color::White);
	ai.setSize(Vector2f(12, 40));
	ai.setPosition(scrWidth - 50, scrHeight / 2 - 100);

	//ball
	ball.setFillColor(Color::White);
	ball.setSize(bSize);
	ball.setPosition(scrWidth / 2, 0);
	
	b.resize(4);
	b[0].setSize(Vector2f(bSize.x - 2, 1));
	b[1].setSize(Vector2f(bSize.x - 2, 1));
	b[2].setSize(Vector2f(1, bSize.y - 2));
	b[3].setSize(Vector2f(1, bSize.y - 2));

	//line
	line.setFillColor(Color::White);
	line.setSize(Vector2f(5, 20));

	//text setup
	points.setCharacterSize(30);
	points.setFillColor(Color::White);
	points.setFont(f);
}

void Pong::draw(RenderWindow& window)
{
	window.clear(Color::Black);

	//draw line
	for (int i = 0; i < scrHeight / line.getSize().y / 2; i++) {
		line.setPosition(scrWidth / 2, i * (line.getSize().y * 2) + line.getSize().y / 2);
		window.draw(line);
	}

	back.draw(window);
	window.draw(player);
	window.draw(ball);
	window.draw(ai);

	//draw scores
	std::string text2;
	text2 = std::to_string(score1);
	points.setString(text2);
	points.setPosition(scrWidth / 4, 10);
	window.draw(points);

	text2 = std::to_string(score2);
	points.setString(text2);
	points.setPosition(scrWidth / 4  * 3, 10);
	window.draw(points);

}

void Pong::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e, float delta)
{

	//clicky boi
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (e.type == Event::KeyPressed) {
			if (e.key.code == Keyboard::Down) down = 1;
			if (e.key.code == Keyboard::Up) up = 1;
		}
		if (e.type == Event::KeyReleased) {
			if (e.key.code == Keyboard::Down) down = 0;
			if (e.key.code == Keyboard::Up) up = 0;
		}
	}
	if (back.isClicked(mouse, window)) gameState = state::menu;

	//movement
	if (up) speedy = -csped;
	else if (down) speedy = csped;
	else speedy = 0;

	//ai movement
	aiMove();
	if (aiu) aiy = -aisped;
	else if (aid) aiy = aisped;
	else aiy = 0;

	//ball stuff

	ty = 0, tx = 0;
	bool x = 0, y = 0;
	//collision
	b[0].setPosition(ball.getPosition().x + 1, ball.getPosition().y);
	b[1].setPosition(ball.getPosition().x + 1, ball.getPosition().y + bSize.y - 1);
	b[2].setPosition(ball.getPosition().x, ball.getPosition().y + 1);
	b[3].setPosition(ball.getPosition().x + ball.getSize().x - 1, ball.getPosition().y + 1);

	//b0 & b1
	if (b[0].getGlobalBounds().intersects(player.getGlobalBounds()) &&
		b[0].getPosition().x > player.getPosition().x + player.getSize().x)	y = 1;
	else if (b[1].getGlobalBounds().intersects(ai.getGlobalBounds()) &&
		b[0].getPosition().x < ai.getPosition().x) y = 1;

	else if (b[1].getGlobalBounds().intersects(player.getGlobalBounds()) &&
		b[1].getPosition().x > player.getPosition().x + player.getSize().x) y = 1;
	else if (b[1].getGlobalBounds().intersects(ai.getGlobalBounds()) &&
		b[1].getPosition().x < ai.getPosition().x) y = 1;

	//b2 & b3
	if (b[2].getGlobalBounds().intersects(player.getGlobalBounds()))	x = 1;
	else if (b[3].getGlobalBounds().intersects(ai.getGlobalBounds())) x = 1;

	//update
	if (x) {
		bspedx = -bspedx;
		speedy = 0;
		tx = 15 * bspedx;
		ball.setPosition(prevPos);

		sound.setBuffer(col); sound.play();
	}

	//ball border
	if (ball.getPosition().y < 0) {
		bspedy = -bspedy;
		sound.setBuffer(col); sound.play();
	}
	if (ball.getPosition().y + ball.getSize().y > scrHeight) {
		bspedy = -bspedy;
		sound.setBuffer(col); sound.play();
	}
	//ball reset + score
	if (ball.getPosition().x < 0) {
		ball.setPosition(bpos[1]);
		bspedx = orgx2; bspedy = orgy2;
		score2++;
		
		sound.setBuffer(death); sound.play();
	}
	else if (ball.getPosition().x + ball.getSize().x > scrWidth) {
		ball.setPosition(bpos[0]);
		bspedx = orgx; bspedy = orgy;
		score1++;

		sound.setBuffer(death); sound.play();
	}

	prevPos = ball.getPosition();

	ball.move(bspedx + tx, bspedy + ty);
	//border stuff
	if (player.getPosition().y < 0) player.setPosition(player.getPosition().x, 0);
	if (player.getPosition().y + player.getSize().y > scrHeight)
	player.setPosition(player.getPosition().x, scrHeight - player.getSize().y);

	if (ai.getPosition().y < 0) ai.setPosition(ai.getPosition().x, 0);
	if (ai.getPosition().y + ai.getSize().y > scrHeight)
		ai.setPosition(ai.getPosition().x, scrHeight - ai.getSize().y);
		
	player.move(0, speedy);
	ai.move(0, aiy);
}

void Pong::aiMove()
{
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<float> dist(0, 1);

	float rand = dist(engine);
	if (ball.getPosition().y - ball.getSize().y / 2 > ai.getPosition().y && rand > prob) {
		aid = 1;
		aiu = 0;
	}
	else if(ball.getPosition().y - ball.getSize().y / 2 < ai.getPosition().y && rand > prob) {
		aiu = 1;
		aid = 0;
	}
	else {
		aiu = 0;
		aid = 0;
	}
}
