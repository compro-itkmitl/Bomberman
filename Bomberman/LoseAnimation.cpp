#include "LoseAnimation.h"
#include <SFML\Graphics.hpp>


LoseAnimation::LoseAnimation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime)
{
	this->imageCount = imageCount;
	this->switchTime = switchTime;
	totalTime = 0.0f;
	currentImage.x = 0;

	uvRect.width = texture->getSize().x / imageCount.x;
	uvRect.height = texture->getSize().y / imageCount.y;

}



LoseAnimation::~LoseAnimation()
{
}


void LoseAnimation::Update(int row, float deltaTime, float start_time)
{
	currentImage.y = row;

	currentImage.x = ((int)((deltaTime - start_time) / switchTime) % imageCount.x);
	uvRect.left = (currentImage.x) * abs(uvRect.width);

}