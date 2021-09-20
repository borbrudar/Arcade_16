#include "Simon.h"

Simon::Simon(Font& f)
{
	//scores
	scr.setPosition(20, 10);
	scr.setCharacterSize(20);
	scr.setFillColor(Color::White);
	scr.setFont(f);

	//read from file
	std::string hsc;
	hs.open("res/simon/hs.txt");
	std::getline(hs, hsc);
	highscore = std::stoi(hsc);
	hs.close();

	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14);

	//sound
	rb.loadFromFile("res/simon/red.wav"); red.setBuffer(rb);
	gb.loadFromFile("res/simon/green.wav"); green.setBuffer(gb);
	bb.loadFromFile("res/simon/blue.wav"); blue.setBuffer(bb);
	yb.loadFromFile("res/simon/yellow.wav"); yellow.setBuffer(yb);
	eb.loadFromFile("res/simon/end.wav"); end.setBuffer(eb);

	//boxes
	boxes.resize(4);
	for (int i = 0; i < boxes.size(); i++) {
		boxes[i].box.setSize(bSize);
		boxes[i].box.setPosition(Vector2f(start.x + bSize.x * (i % 2), start.y + bSize.y * std::floor(i / 2)));
	}

	boxes[0].setup(Color::Red);
	boxes[1].setup(Color::Green);
	boxes[2].setup(Color::Blue);
	boxes[3].setup(Color::Yellow);

	//first instruction
	inst();
}

void Simon::draw(RenderWindow& window)
{
	back.draw(window);

	//boxes
	for (int i = 0; i < boxes.size(); i++) window.draw(boxes[i].box);

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
	scr.setPosition(150, 10);
	window.draw(scr);
}

void Simon::update(Mouse& mouse, RenderWindow& window, state& gameState, Event& e)
{
	if (score > highscore) {
		highscore = score;
		//write to file and close it
		hs.open("res/simon/hs.txt", std::ios::out | std::ios::trunc);
		hs << highscore;
		hs.close();
	}

	//click click
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed) window.close();
		if (Mouse::isButtonPressed(Mouse::Button::Left)) {
			//check for touch
			for (int i = 0; i < boxes.size(); i++) {
				if (boxes[i].box.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
					//win/lose 
					if (!won) {
						//set the colors straight
						boxes[i].bright();
						music(i);
						moves.push_back(i);
						if (moves.size() == instructions.size() &&
							moves.back() == instructions[moves.size() - 1]) {
							score++;
							inst();
							moves.clear();
							won = 1;
						}
						else if (!(moves.back() == instructions[moves.size() - 1])) {
							stop();
							end.play();
							score = 0;
							won = 1;
							inst(1);
							moves.clear();
						}
					}
				}
			}
		}
	}

	if (back.isClicked(mouse, window)) gameState = state::menu;

	//slowly subtract colors
	for (int i = 0; i < boxes.size(); i++) boxes[i].update();

	//demonstrate instructions
	if (won) {
		itimer = iclock.getElapsedTime().asSeconds();
		itime += itimer;
		iclock.restart();

		if (itime > idelay) {
			//demonstration
			itime = 0;
			boxes[instructions[ind]].bright();
			music(instructions[ind]);

			if ((ind + 1)== instructions.size()) {
				won = 0;
				ind = 0;
			}
			else ind++;
		}
	}
	else {
		iclock.restart();
		itime = 0.f;
	}

}

void Simon::inst(bool reset)
{
	if (reset) instructions.clear();
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<int> dist(0, 3);
	instructions.push_back(dist(engine));

}

void Simon::music(int type)
{
	if (type == 0) red.play();
	else if (type == 1) green.play();
	else if (type == 2) blue.play();
	else if (type == 3) yellow.play();
}

void Simon::stop()
{
	red.stop();
	green.stop();
	blue.stop();
	yellow.stop();
}

void BoxS::setup(Color col)
{
	oldr = col.r * k;
	oldg = col.g * k;
	oldb = col.b * k;

	box.setFillColor(Color(oldr, oldg, oldb));
}

void BoxS::bright()
{
	float r = 0, g = 0, b = 0;
	if (oldr * mult > 255) r = 255; else r = oldr * mult;
	if (oldg * mult > 255) g = 255; else g = oldg * mult;
	if (oldb * mult > 255) b = 255; else b = oldb * mult;

	box.setFillColor(Color(r, g, b));
}

void BoxS::update()
{
	//slowly subtract colors
	if (box.getFillColor().r > oldr) box.setFillColor(Color(box.getFillColor().r - sub,
		box.getFillColor().g, box.getFillColor().b));

	if (box.getFillColor().g > oldg) box.setFillColor(Color(box.getFillColor().r,
		box.getFillColor().g - sub, box.getFillColor().b));
	
	if (box.getFillColor().b > oldb) box.setFillColor(Color(box.getFillColor().r,
		box.getFillColor().g, box.getFillColor().b - sub));
}
