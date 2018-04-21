#pragma once
#include <SFML\Graphics.hpp>
class LoseAnimation
{
public:
	LoseAnimation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
	~LoseAnimation();
	void Update(int row, float deltaTime, float start_time);

public:
	sf::IntRect uvRect;

private:
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;

	float totalTime;
	float switchTime;


};

