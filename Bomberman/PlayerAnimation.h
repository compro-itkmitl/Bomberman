#pragma once
#include <SFML\Graphics.hpp>

class PlayerAnimation
{
public:
	PlayerAnimation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
	~PlayerAnimation();

	void Update(int row, float deltaTime, int face);	

public:
	sf::IntRect uvRect;

private:
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;

	float totalTime;
	float switchTime;
};

