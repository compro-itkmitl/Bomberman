#pragma once
#include <SFML\Graphics.hpp>

class Collider
{
public:
	Collider(sf::RectangleShape& body);
	~Collider();

	void Move(float dx, float dy) { body.move(dx, dy); }
	 
	bool CheckCollision(Collider& other, float push, int mytype, int othertype);

	sf::Vector2f GetPosition() { return body.getPosition(); }
	sf::Vector2f GetHalfSize(int type) { 
		if (type == 0)
			return body.getSize() / 2.0f;
		if (type == 1)
			return sf::Vector2f(22.0f, 20.0f);
		if (type == 2)
			return sf::Vector2f(25.0f, 25.0f);
		if (type == 3)
			return sf::Vector2f(15.0f, 15.0f);
		return sf::Vector2f(25.0f, 25.0f);
	}

private:
	sf::RectangleShape& body;
}; 

