#pragma once
#include <SFML\Graphics.hpp>
#include "BombExplodingAnim.h"
#include "Collider.h"
class BombExploding
{
public:
	BombExploding(sf::Texture* texture, sf::Vector2f bomb_pos, sf::Vector2u imageCount, float switchTime);
	~BombExploding();
	void Update(float start_time, float deltaTime, float switchTime);
	void Draw(sf::RenderWindow& window);

	sf::Vector2f GetPosition() { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }

private:
	sf::RectangleShape body;
	BombExplodingAnim animation;
	unsigned int row;
	float speed;
	
};

