#include "BombAnimation.h"
#include <SFML\Graphics.hpp>


BombAnimation::BombAnimation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime)
{
	this->imageCount = imageCount;
	this->switchTime = switchTime;
	totalTime = 0.0f;
	currentImage.x = 0;

	uvRect.width = texture->getSize().x / imageCount.x;
	uvRect.height = texture->getSize().y / imageCount.y;

}


BombAnimation::~BombAnimation()
{
}

void BombAnimation::Update(int row, float deltaTime, float start_time)
{
	currentImage.y = row;

	/*
	totalTime = deltaTime;
	
	if (totalTime >= switchTime)
	{
		totalTime -= switchTime;
		currentImage.x++;

		if (currentImage.x >= imageCount.x)
		{
			currentImage.x = 0;
		}
	}
	*/
	currentImage.x = ((int)((deltaTime - start_time) / switchTime) % imageCount.x);
	uvRect.left = (currentImage.x) * abs(uvRect.width);

}