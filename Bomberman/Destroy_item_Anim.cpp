#include "Destroy_item_Anim.h"
#include <SFML\Graphics.hpp>


Destroy_item_Anim::Destroy_item_Anim(sf::Texture* texture, sf::Vector2u imageCount, float switchTime)
{
	this->imageCount = imageCount;
	this->switchTime = switchTime;
	totalTime = 0.0f;
	currentImage.x = 0;

	uvRect.width = texture->getSize().x / imageCount.x;
	uvRect.height = texture->getSize().y / imageCount.y;

}


Destroy_item_Anim::~Destroy_item_Anim()
{
}

void Destroy_item_Anim::Update(int row, float deltaTime, float start_time)
{
	currentImage.y = row;
	currentImage.x = ((int)((deltaTime - start_time) / switchTime) % imageCount.x);
	uvRect.left = (currentImage.x) * abs(uvRect.width);

}