#pragma once
#include <SFML\Graphics.hpp>
#include "PlayerAnimation.h"
#include "Collider.h"
class Player2rd
{
public:
	Player2rd(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, float speed, sf::Vector2f pos);
	~Player2rd();

	void Update(float deltaTime, float switchTime, int speed);
	void Draw(sf::RenderWindow& window);

	sf::Vector2f GetPosition() { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }

	int planting = 0;

private:
	sf::RectangleShape body;
	PlayerAnimation animation;
	unsigned int row;
	float speed;
	int face;

};

