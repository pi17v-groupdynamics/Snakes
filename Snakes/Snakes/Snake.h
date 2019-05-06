#pragma once
#include <iostream>
#include <vector>
class Snake
{
public:
	enum  direction{ up, down, left, right};
	std::pair<int, int> head;
	std::vector<std::pair<int,int>> tail;
	direction dir;
private:
	int id;
public:
	void move();
	Snake(Snake::direction dir, int id);
	int handleInput();
	~Snake();
};

