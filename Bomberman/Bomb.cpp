#include "Bomb.h"
#include <SFML\Graphics.hpp>


Bomb::Bomb(sf::Texture* texture, sf::Vector2f bomb_pos, sf::Vector2u imageCount, float switchTime) :
	animation(texture, imageCount, switchTime)
{
	this->speed = speed;
	row = 0;

	body.setSize(sf::Vector2f(40.0f, 40.0f));
	//body.setOrigin(body.getSize() / 2.0f);
	body.setOrigin(sf::Vector2f(20.0f, 20.0f));
	body.setPosition(bomb_pos);
	body.setTexture(texture);

}


Bomb::~Bomb()
{

}

void Bomb::Update(float start_time, float deltaTime, float switchTime)
{
	animation.Update(row, deltaTime, start_time);
	body.setTextureRect(animation.uvRect);
}

void Bomb::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
