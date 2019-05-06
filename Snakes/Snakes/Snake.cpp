#include "Snake.h"
#include <SFML/Graphics.hpp>

template <typename T>
std::pair<T, T> clamp(std::pair<T, T> p, T min, T max) {
	auto x = p.first > max ? min : p.first;
	x = p.first < min ? max : p.first;
	auto y = p.second > max ? min : p.second;
	y = p.second < min ? max : p.second;
	return { x, y };
}

int Snake::handleInput()
{
	if (id == 1) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			if (dir != right)
				dir = left;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			if (dir != down)
				dir = up;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			if (dir != left)
				dir = right;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			if (dir != up)
				dir = down;
	}
	else if (id == 2) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			if(dir!=right)
				dir = left;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			if (dir != down)
				dir = up;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			if (dir != left)
				dir = right;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			if (dir != up)
				dir = down;
	}
	return dir;
}

void Snake::move()
{
	int& last_tail_x = tail[tail.size() - 1].first;
	int& last_tail_y = tail[tail.size() - 1].second;

	last_tail_x = head.first;
	last_tail_y = head.second;

	if (dir == direction::left) head.first -= 1;
	else if (dir == direction::right) head.first += 1;
	else if (dir == direction::up) head.second -= 1;
	else if (dir == direction::down) head.second += 1;
	
	if (head.first > 31)
		head.first = 0;
	else if (head.first < 0)
		head.first = 31;
	else if (head.second > 31)
		head.second = 0;
	else if (head.second < 0)
		head.second = 31;
	
	head = clamp<int>(head, 0, 31);
	std::rotate(tail.rbegin(), tail.rbegin() +1, tail.rend());
}

Snake::Snake(Snake::direction dir, int id)
{
	this->dir = dir;
	this->id = id;
}


Snake::~Snake()
{
}
