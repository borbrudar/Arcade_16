#include "Tetris.h"


Tetris::Tetris(Font& f)
{
	//read highscore from file
	std::string hsc;
	highscore.open("res/tetris/highscore.txt");
	std::getline(highscore, hsc);
	highScore = std::stoi(hsc); 
	highscore.close();

	//setup theme song
	theme.openFromFile("res/tetris/theme.wav");
	theme.play();
	theme.setLoop(true);
	
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20), 14); 

	//setup score
	points.setFillColor(Color::White);
	points.setCharacterSize(28);
	points.setFont(f);

	//setup background
	playfield.loadFromFile("res/tetris/playfield.png");
	fieldy.setTexture(playfield);
	fieldy.setPosition((startX - 1) * 18, (startY - 1) * 18);
	//setup tiles
	til.loadFromFile("res/tetris/tiles.png");
	tiles.setTexture(til);

	gamey.loadFromFile("res/tetris/gameOver.png");
	game.setTexture(gamey);
	game.setPosition(100, 180);

	newPiece();
}

Tetris::~Tetris()
{
	//write to file and close it
	highscore.open("res/tetris/highscore.txt", std::ios::out | std::ios::trunc);
	if (score > highScore) highScore = score;
	highscore << highScore;
	highscore.close();

	theme.stop();
}

void Tetris::draw(RenderWindow& window)
{
	//draw button
	back.draw(window);

	//draw score and highscore
	std::string text2;
	text2.assign("SCORE: ");
	text2 = text2 + std::to_string(score);
	points.setString(text2);
	points.setPosition(340, 150);
	window.draw(points);

	text2.assign("HIGHSCORE: ");
	text2 = text2 + std::to_string(highScore);
	points.setString(text2);
	points.setPosition(340, 185);
	window.draw(points);

	//draw field
	window.draw(fieldy);
	for (int y = 0; y < M; y++) {
		for (int x = 0; x < N; x++) {
			if (field[y][x] != 0){
				tiles.setTextureRect(IntRect((field[y][x] - 1) * 18, 0, 18, 18));
				tiles.setPosition((x + startX) * 18, (y + startY) * 18);
				window.draw(tiles);
			}
		}
	}

	tiles.setTextureRect(IntRect(type * 18, 0, 18, 18));
	//draw tiles
	for (int i = 0; i < 4; i++) {
		tiles.setPosition(a[i].x * 18, a[i].y * 18);
		window.draw(tiles);
	}

	if (gameOver) window.draw(game);
}

void Tetris::update(Mouse& mouse, RenderWindow& window, state& gameState, Event &e)
{
	if (!gameOver) {
		//clock setup
		time = clock.getElapsedTime().asSeconds();
		timer += time;
		clock.restart();
		delay = 0.2f;

		//clicky boi
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Up) rot = 1;
				else if (e.key.code == Keyboard::Left) dx = -1;
				else if (e.key.code == Keyboard::Right) dx = 1;
				else if (e.key.code == Keyboard::Down) delay = 0.03f;
			}
		}
		if (back.isClicked(mouse, window)) { 
			theme.stop();
			gameState = state::menu; 
		}

		//before move/rotation so we can adjust
		for (int i = 0; i < 4; i++) b[i] = a[i];

		//rotation
		if (rot) {
			point p = a[1]; //center of rotation
			for (int i = 0; i < 4; i++) {
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
		}

		//move
		for (int i = 0; i < 4; i++) a[i].x += dx;

		//check border + piece collision
		checkBorder();

		//fall
		if (timer > delay && fall) {
			for (int i = 0; i < 4; i++) a[i].y++; timer = 0;
		}
		//hitting ground
		else if (timer > groundDelay) {
			timer = 0; fall = 1;
			for (int i = 0; i < 4; i++) {
				field[a[i].y - startY][a[i].x - startX] = type + 1; //cannot initalize an array to -1
			}
			//update board
			updateField();
			//create new piece
			newPiece();
		}

		//reset movement and rotation
		dx = 0; rot = 0;

		if (score > highScore) highScore = score;
		//check if game over
		for (int i = 0; i < N; i++) {
			if (field[0][i] != 0) {
				gameOver = 1;
				if (score > highScore) highScore = score;
				break;
			}
		}
	}
	else {
		//clicky boi
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::R) { 
					gameOver = 0; 
					score = 0;
					for (int x = 0; x < M; x++) {
						for (int y = 0; y < N; y++) {
							field[x][y] = 0;
						}
					}
					clock.restart();
				}
			}
		}
		if (back.isClicked(mouse, window)) gameState = state::menu;
	}
}

void Tetris::checkBorder()
{ 
	//border
	for (int i = 0; i < 4; i++) {
		if (a[i].x == (N + startX)) for (int i = 0; i < 4; i++) a[i].x--;
		if (a[i].x < startX) for (int i = 0; i < 4; i++) a[i].x++;

		if (a[i].y == (M - 1 + startY)) fall = 0;
	}

	//fall 
	for (int i = 0; i < 4; i++) {
		if (field[a[i].y + 1 - startY][a[i].x - startX] == 0) fall = 1; 
		else {
			fall = 0;
			break;
		}
	}
	
	//other pieces 
	for (int i = 0; i < 4; i++) {
		if (field[a[i].y - startY][a[i].x - startX] != 0) {
			for (int k = 0; k < 4; k++) a[k] = b[k];
			break;
		}
	}
}

void Tetris::newPiece()
{
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<int> dist(0, 6);
	type = dist(engine);
	tiles.setTextureRect(IntRect(type * 18, 0, 18, 18));

	for (int i = 0; i < 4; i++) {
		a[i].x = shapes[type][i] % 2 + startX + (N / 2 - 1);
		a[i].y = shapes[type][i] / 2 + startY - 1;
	}
}

void Tetris::updateField()
{
	//setup something somethin
	int count = 0, row = 0, k;
	bool temp = 1;
	//check 4 times (max lines you can delete at once)
	for (int i = 0; i < 4; i++) {
		if (temp == 0) break;
		//basically check from bottom to top, and if true, replace every line with the one above it
		for (int y = (M - 1); y > 0; y--) {
			k = y - 1;
			row = 0;
			//check one row
			for (int x = 0; x < N; x++) {
				if (field[y][x] != 0) row++;
			}
			//delete row
			if (row == N) {
				count++;
				temp = 1;
				for (int f = y; f > 0; f--) {
					for (int x = 0; x < N; x++) field[f][x] = field[k][x];
					k--;
				}
			}
			else temp = 0;
		}
	}

	//add to the score
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<int> dist(90, 150);

	score += dist(engine) * count;
	if (count == 4) score += dist(engine) * count;
}
