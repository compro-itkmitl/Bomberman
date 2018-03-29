#pragma once
#include <SFML\Graphics.hpp>

	class BombAnimation
{
public:
	BombAnimation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
	~BombAnimation();

	void Update(int row, float deltaTime);

public:
	sf::IntRect uvRect;

private:
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;

	float totalTime;
	float switchTime;
};

