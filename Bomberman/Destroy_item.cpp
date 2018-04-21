#include "Destroy_item.h"
#include <SFML\Graphics.hpp>


Destroy_item::Destroy_item(sf::Texture* texture, sf::Vector2f bomb_pos, sf::Vector2u imageCount, float switchTime) :
	animation(texture, imageCount, switchTime)
{
	this->speed = speed;
	row = 0;

	body.setSize(sf::Vector2f(40.0f, 60.0f));
	//body.setOrigin(body.getSize() / 2.0f);
	body.setOrigin(sf::Vector2f(20.0f, 35.0f));
	body.setPosition(bomb_pos);
	body.setTexture(texture);

}
Destroy_item::~Destroy_item()
{
}

void Destroy_item::Update(float start_time, float deltaTime, float switchTime)
{
	animation.Update(row, deltaTime, start_time);
	body.setTextureRect(animation.uvRect);
}

void Destroy_item::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}