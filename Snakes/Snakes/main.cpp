#include <SFML/Graphics.hpp>
#include "windows.h"
#include "Snake.h"
enum tile { none = 0, snake, snake2, fruit, block, god_apple, devil_apple };

int countApplesMap = 0;
int CountBadApples = 0;
void generateApple(tile tiles[32][32]) {
	int x;
	int y;
	try_again:
	x = rand() % 32;
	y = rand() % 32;
	if (tiles[y][x] == none) {
		int random = rand() % 100;
		tiles[y][x] = fruit;
		countApplesMap++;

		if (random < 45 && CountBadApples<20) {
			tiles[y][x] = devil_apple;
			countApplesMap--;
			CountBadApples++;
			generateApple(tiles);
		}
		//else generateApple(tiles);
		if (random < 15) {
			tiles[y][x] = god_apple;
			countApplesMap--;
		}

	}
	else goto try_again;
}

tile NextMove(tile tiles[32][32], Snake& snake, Snake& other_snake) {
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

	//point, index
	std::vector<std::pair<std::pair<int, int>, int>> points;
	int i = 0;
	for (auto point : other_snake.tail) {
		if (head.first == point.first && head.second == point.second) {
			points.push_back({ point, i });
		}
		i++;
	}
	for (auto point : points) {
		if (point.second > 3) {
			std::vector<std::pair<int, int>>(other_snake.tail.begin(), other_snake.tail.begin() + point.second).swap(other_snake.tail);
			for (int i = 0; i < point.second; i++) {
				generateApple(tiles);
			}
		}
	}

	return tiles[head.second][head.first];
}

int main()
{
	ShowWindow(GetConsoleWindow(), 0);

	int player1score = 0;
	int player2score = 0;
	int goal = 100;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Snakes");
	////512x75
	sf::RectangleShape scoreBoardBackground;
	scoreBoardBackground.setFillColor(sf::Color::White);
	scoreBoardBackground.setSize({ 512, 77 });
	scoreBoardBackground.setPosition(0, 600-75 - 2);
	
	sf::Font font;
	font.loadFromFile("arial.ttf");

	sf::Texture texture;
	texture.loadFromFile("andermirik.jpg");
	sf::Sprite image;
	image.setTexture(texture);
	image.setPosition(800 - 278, 0);


	sf::Text text("player 1: 0", font, 20);
	text.setFillColor(sf::Color(46, 176, 253));
	text.setPosition(0 + 15, 600-77+10);

	sf::Text text2("player 2: 0", font, 20);
	text2.setFillColor(sf::Color(42, 42, 114));
	text2.setPosition(0 + 15, 600-77+10 + 30);

	sf::Text text_goal("goal:"+std::to_string(goal), font, 20);
	text_goal.setFillColor(sf::Color(0,0,0));
	text_goal.setPosition(412 + 15, 600 - 77 + 10);


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
	for (int i = 0; i < 20; i++) {
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
				else if (tile_now == tile::god_apple) {
					tile_draw.setFillColor(sf::Color(253, 233, 16));
				}
				else if (tile_now == tile::devil_apple) {
					tile_draw.setFillColor(sf::Color(199, 0, 0));
				}
				tile_draw.setPosition({ j*16.0f, i*16.0f });
				window.draw(tile_draw);
			}
		}
		//draw 1 snake

		tile_draw.setFillColor(sf::Color(46, 176, 253));
		tile_draw.setPosition({ snake.head.first*16.0f, snake.head.second*16.0f });
		if(snake.tail.size()!=0)
		window.draw(tile_draw);
		for (auto coord : snake.tail) {
			tile_draw.setFillColor(sf::Color(46, 176, 253));
			tile_draw.setPosition({ coord.first*16.0f, coord.second*16.0f });
			window.draw(tile_draw);
		}
		//draw 2 snake
		tile_draw.setFillColor(sf::Color(42, 42, 114));
		tile_draw.setPosition({ snake2.head.first*16.0f, snake2.head.second*16.0f });
		if (snake2.tail.size() != 0)
		window.draw(tile_draw);
		for (auto coord : snake2.tail) {
			tile_draw.setFillColor(sf::Color(42, 42, 114));
			tile_draw.setPosition({ coord.first*16.0f, coord.second*16.0f });
			window.draw(tile_draw);
		}

		snake.handleInput();
		snake2.handleInput();

		elapsed += clock.getElapsedTime().asMilliseconds();
		if (elapsed >= 4400) {
			switch(NextMove(tiles, snake, snake2)) {
			case tile::block:
				break;
			case tile::none:
				snake.move();
				break;
			case tile::fruit:
				snake.move();
				tiles[snake.head.second][snake.head.first] = none;
				snake.tail.push_back(snake.tail[snake.tail.size() - 1]);
				if (snake.tail.size() == 14) {
					std::vector<std::pair<int, int>>(snake.tail.begin(), snake.tail.begin() + 4).swap(snake.tail);
					player1score += 10;
				}
				countApplesMap--;
				if(countApplesMap<=20)
					generateApple(tiles);
				break;
			case tile::god_apple:
				snake.move();
				tiles[snake.head.second][snake.head.first] = none;
				player1score += 5;
				generateApple(tiles);
				break;
			case tile::devil_apple:
				snake.move();
				tiles[snake.head.second][snake.head.first] = none;
				player1score = std::max(0, player1score-10);
				generateApple(tiles);
				break;
			default:
				break;
			}
			//длина 15 -> обрезать до 5 -> добавить в счёт
			switch (NextMove(tiles, snake2, snake)) {
			case tile::block:
				break;
			case tile::none:
				snake2.move();
				break;
			case tile::fruit:
				snake2.move();
				tiles[snake2.head.second][snake2.head.first] = none;
				snake2.tail.push_back(snake2.tail[snake2.tail.size() - 1]);
				if (snake2.tail.size() == 14){
					std::vector<std::pair<int, int>>(snake2.tail.begin(), snake2.tail.begin() + 4).swap(snake2.tail);
					player2score += 10;
				}
				countApplesMap--;
				if (countApplesMap <= 20)
					generateApple(tiles);
				break;
			case tile::god_apple:
				snake2.move();
				tiles[snake2.head.second][snake2.head.first] = none;
				player2score += 5;
				generateApple(tiles);
				break;
			case tile::devil_apple:
				snake2.move();
				tiles[snake2.head.second][snake2.head.first] = none;
				player2score = std::max(0, player2score - 10);
				generateApple(tiles);
				break;
			default:
				break;
			}
			text.setString("player1: " + std::to_string(player1score));
			text2.setString("player2: " + std::to_string(player2score));

			goal = 100;
			if (player1score >= goal) {
				for (int j = 0; j < 32; j++) {
					for (int i = 0; i < 32; i++) {
						if(tiles[j][i]!=tile::block)
						tiles[j][i] = tile::snake;
					}
				}
				snake.tail.clear();
				snake2.tail.clear();
			}
			else if (player2score >= goal) {
				for (int j = 0; j < 32; j++) {
					for (int i = 0; i < 32; i++) {
						if (tiles[j][i] != tile::block)
							tiles[j][i] = tile::snake2;
					}
				}
				snake.tail.clear();
				snake2.tail.clear();
			}

			clock.restart();
			elapsed = 0;
		}

		
		window.draw(image);

		window.draw(scoreBoardBackground);
		window.draw(text);
		window.draw(text2);
		window.draw(text_goal);
		window.display();
	}
	return 0;
}