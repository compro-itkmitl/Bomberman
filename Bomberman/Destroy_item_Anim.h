#pragma once
#include <SFML\Graphics.hpp>

class Destroy_item_Anim
{

public:
	Destroy_item_Anim(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
	~Destroy_item_Anim();

	void Update(int row, float deltaTime, float start_time);

public:
	sf::IntRect uvRect;

private:
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;

	float totalTime;
	float switchTime;
};

