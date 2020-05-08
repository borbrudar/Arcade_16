#include "Tetris.h"


Tetris::Tetris(Font& f)
{
	//setup back button
	std::string text1;
	text1.assign("Back");
	back.setup(f, text1, Color::Blue, Vector2f(50, 30), Vector2f(580, 20)); 

	//setup tiles
	til.loadFromFile("res/tetris/tiles.png");
	tiles.setTexture(til);

	newPiece();
}

void Tetris::draw(RenderWindow& window)
{
	//draw button
	back.draw(window);

	//draw field
	for (int y = 0; y < M; y++) {
		for (int x = 0; x < N; x++) {
			if (field[y][x] != 0){
				tiles.setTextureRect(IntRect((field[y][x] - 1) * 18, 0, 18, 18));
				tiles.setPosition(x * 18, y * 18);
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
}

void Tetris::update(Mouse& mouse, RenderWindow& window, state& gameState, Event &e)
{
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
	if (back.isClicked(mouse,window)) gameState = state::menu;
	
	//before move/rotation so we can adjust
	for (int i = 0; i < 4; i++) b[i] = a[i];

	//rotation
	if(rot){
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

	//fall
	if (timer > delay && fall) {
		for (int i = 0; i < 4; i++) a[i].y++; timer = 0;
	}
	//hitting ground
	else if (timer > groundDelay) {
		timer = 0; fall = 1;
		for (int i = 0; i < 4; i++) {
			field[a[i].y][a[i].x] = type + 1; //cannot initalize an array to -1
		}
		//update board
		updateField();
		//create new piece
		newPiece();
	}

	//check border + piece collision
	checkBorder();

	//reset movement and rotation
	dx = 0; rot = 0;
}

void Tetris::checkBorder()
{ 
	//border
	for (int i = 0; i < 4; i++) {
		if (a[i].x == N - 1) for (int i = 0; i < 4; i++) a[i].x--;
		if (a[i].x < 0) for (int i = 0; i < 4; i++) a[i].x++;

		if (a[i].y == (M - 1)) fall = 0;
	}

	//fall 
	for (int i = 0; i < 4; i++) {
		if (field[a[i].y + 1][a[i].x] == 0) fall = 1; 
		else {
			fall = 0;
			break;
		}
	}
	
	//other pieces 
	for (int i = 0; i < 4; i++) {
		if (field[a[i].y][a[i].x] != 0) {
			for (int k = 0; k < 4; k++) a[k] = b[k];
			break;
		}
	}
}

void Tetris::newPiece()
{
	srand(std::time(NULL));
	int n = rand() % 7;
	type = n;
	tiles.setTextureRect(IntRect(n * 18, 0, 18, 18));
	
	for (int i = 0; i < 4; i++) {
		a[i].x = shapes[n][i] % 2;
		a[i].y = shapes[n][i] / 2;
	}
}

void Tetris::updateField()
{
	//setup something somethin
	int row = 0;
	int k;

	//basically check from bottom to top, and if true, replace every line with the one above it
	for (int y = M - 1; y > 0; y--) {
		k = y - 1;
		row = 0;
		//check one row
		for (int x = 0; x < N; x++) {
			if (field[y][x] != 0) row++;
		}
		//delete row
		if (row == (N - 1)) {
			for (int f = y; f > 0; f--) {
				for (int x = 0; x < N; x++) field[f][x] = field[k][x];				
				k--;
			}
		}
	}
}
