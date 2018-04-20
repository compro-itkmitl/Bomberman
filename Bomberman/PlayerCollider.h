#pragma once
#include <SFML\Graphics.hpp>
#include "Collider.h"
class PlayerCollider
{
public:
	PlayerCollider(sf::RectangleShape& body);
	~PlayerCollider();

	void Move(float dx, float dy) { body.move(dx, dy); }

	bool CheckCollision(Collider& other, float push);

	sf::Vector2f GetPosition() { return body.getPosition(); }
	sf::Vector2f GetHalfSize() { return body.getSize() / 4.0f; }

private:
	sf::RectangleShape& body;
};

