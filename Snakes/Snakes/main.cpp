#include <SFML/Graphics.hpp>
#include "windows.h"
#include "Snake.h"
enum tile { none = 0, snake, snake2, fruit, block };

void generateApple(tile tiles[32][32]) {
	int x;
	int y;
	try_again:
	x = rand() % 32;
	y = rand() % 32;
	if (tiles[y][x] == none)
		tiles[y][x] = fruit;
	else goto try_again;
}

tile NextMove(tile tiles[32][32], Snake& snake) {
	auto dir = snake.dir;
	auto head = snake.head;
	if (dir == Snake::direction::left) head.first -= 1;
	else  if (dir == Snake::direction::right) head.first += 1;
	else if (dir == Snake::direction::up) head.second -= 1;
	else if (dir == Snake::direction::down) head.second += 1;

	if (head.first > 31) head.first = 0;
	else if (head.first < 0) head.first = 31;
	else if (head.second > 31) head.second = 0;
	else if (head.second < 0) head.second = 31;
	if (tiles[head.second][head.first] == block)
		return tiles[head.second][head.first];
		
	return tiles[head.second][head.first];
}

int main()
{
	ShowWindow(GetConsoleWindow(), 0);

	sf::RenderWindow window(sf::VideoMode(800, 600), "title");

	
	srand(67656521);
	
	tile tiles[32][32] = {none};//16 pix for 1 tile 512x512 area
	//generate blocks
	for (int i = 0; i < 8; i++) {
		int length = 4 + rand() % 6;
		if (rand() % 2) {//horizontal
			int x = rand() % (32 - length);
			int y = rand() % 32;
			for(int j=0;j<5;j++)
				tiles[32-y][x++] = block;
		}
		else {//vertical
			int y = rand() % (32 - length);
			int x = rand() % 32;
			for (int j = 0; j < 5; j++)
				tiles[32-y++][x] = block;
		}
	}
	//generate apples
	for (int i = 0; i < 10; i++) {
		generateApple(tiles);
	}
	srand(time(0));
	sf::Event event;

	tile tile_now = none;
	sf::RectangleShape tile_draw({ 16.0f, 16.0f });

	Snake snake(Snake::direction::down, 1);
	for(int i=3;i>=0;i--)
		snake.tail.push_back({ 0,i });
	snake.head = { 0,4 };

	Snake snake2(Snake::direction::up, 2);
	for (int i = 3; i >= 0; i--)
		snake2.tail.push_back({ 31,31-i });
	snake2.head = { 31,31-4 };

	sf::Clock clock;
	int elapsed = 0;

	while (window.isOpen()) {
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color(223, 230, 233));
		
		for (int j = 0; j < 32; j++) {
			for (int i = 0; i < 32; i++) {
				tile_now = tiles[i][j];
				if (tile_now == none) {
					tile_draw.setFillColor(sf::Color::White);
				}
				else if (tile_now == tile::fruit) {
					tile_draw.setFillColor(sf::Color(255, 160, 0));
				}
				else if (tile_now == tile::snake) {
					tile_draw.setFillColor(sf::Color(46, 176, 253));
				}
				else if (tile_now == tile::snake2) {
					tile_draw.setFillColor(sf::Color(42, 42, 114));
				}
				else if (tile_now == tile::block) {
					tile_draw.setFillColor(sf::Color(35, 37, 40));
				}
				tile_draw.setPosition({ j*16.0f, i*16.0f });
				window.draw(tile_draw);
			}
		}
		//draw 1 snake
		tile_draw.setFillColor(sf::Color(46, 176, 253));
		tile_draw.setPosition({ snake.head.first*16.0f, snake.head.second*16.0f });
		window.draw(tile_draw);
		for (auto coord : snake.tail) {
			tile_draw.setPosition({ coord.first*16.0f, coord.second*16.0f });
			window.draw(tile_draw);
		}
		//draw 2 snake
		tile_draw.setFillColor(sf::Color(42, 42, 114));
		tile_draw.setPosition({ snake2.head.first*16.0f, snake2.head.second*16.0f });
		window.draw(tile_draw);
		for (auto coord : snake2.tail) {
			tile_draw.setPosition({ coord.first*16.0f, coord.second*16.0f });
			window.draw(tile_draw);
		}

		snake.handleInput();
		snake2.handleInput();

		elapsed += clock.getElapsedTime().asMilliseconds();
		if (elapsed >= 4400) {
			switch(NextMove(tiles, snake)) {
				snake.move();
			}

			switch(NextMove(tiles, snake2)) {
				snake2.move();
			}

			clock.restart();
			elapsed = 0;
		}

		
		
		window.display();

	}

	return 0;
}