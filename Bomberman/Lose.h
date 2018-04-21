#pragma once
#include <SFML\Graphics.hpp>
#include "LoseAnimation.h"
#include "Collider.h"
class Lose
{
public:
	Lose(sf::Texture* texture, sf::Vector2f bomb_pos, sf::Vector2u imageCount, float switchTime);
	~Lose();
	void Update(float start_time, float deltaTime, float switchTime);
	void Draw(sf::RenderWindow& window);

private:
	sf::RectangleShape body;
	LoseAnimation animation;
	unsigned int row;
	float speed;
};

