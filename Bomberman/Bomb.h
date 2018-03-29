#pragma once
#include <SFML\Graphics.hpp>
#include "BombAnimation.h"
#include "Collider.h"
class Bomb
{
public:
	Bomb(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
	~Bomb();

	void Update(float deltaTime, float switchTime);
	void Draw(sf::RenderWindow& window);

	sf::Vector2f GetPosition() { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }

private:
	sf::RectangleShape body;
	BombAnimation animation;
	unsigned int row;
	float speed;
};

