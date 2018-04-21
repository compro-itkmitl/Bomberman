#include "Lose.h"
#include <SFML\Graphics.hpp>

Lose::Lose(sf::Texture* texture, sf::Vector2f bomb_pos, sf::Vector2u imageCount, float switchTime) :
	animation(texture, imageCount, switchTime)
{
	this->speed = speed;
	row = 0;

	body.setSize(sf::Vector2f(50.0f, 120.0f));
	//body.setOrigin(body.getSize() / 2.0f);
	body.setOrigin(sf::Vector2f(25.0f, 105.0f));
	body.setPosition(bomb_pos);
	body.setTexture(texture);

}


Lose::~Lose()
{
}

void Lose::Update(float start_time, float deltaTime, float switchTime)
{
	animation.Update(row, deltaTime, start_time);
	body.setTextureRect(animation.uvRect);
}

void Lose::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}