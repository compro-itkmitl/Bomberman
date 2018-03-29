#include "PlayerAnimation.h"


PlayerAnimation::PlayerAnimation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime)
{
	this->imageCount = imageCount;
	this->switchTime = switchTime;
	totalTime = 0.0f;
	currentImage.x = 0; 

	uvRect.width  = texture->getSize().x / imageCount.x;
	uvRect.height = texture->getSize().y / imageCount.y;
}


PlayerAnimation::~PlayerAnimation()
{
}


void PlayerAnimation::Update(int row, float deltaTime, int face)
{
	currentImage.y = row;
	totalTime += deltaTime;

	if (totalTime >= switchTime)
	{
		totalTime -= switchTime;
		currentImage.x ++;

		if (currentImage.x >= imageCount.x)
		{
			currentImage.x = 0;
		}
	}

	if (face == 99)
	{
		currentImage.x = 0;
		currentImage.y = row;
	}
	else
	{
		uvRect.top = currentImage.y * uvRect.height;
	}
	uvRect.left = (currentImage.x) * abs(uvRect.width);
}

