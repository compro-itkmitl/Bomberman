#include "BombExploding.h"
#include <SFML\Graphics.hpp>


BombExploding::BombExploding(sf::Texture* texture, sf::Vector2f bomb_pos, sf::Vector2u imageCount, float switchTime):
	animation(texture, imageCount, switchTime)
{
	this->speed = speed;
	row = 0;

	body.setSize(sf::Vector2f(52.0f, 52.0f));
	//body.setOrigin(body.getSize() / 2.0f);
	body.setOrigin(sf::Vector2f(25.0f, 25.0f));
	body.setPosition(bomb_pos);
	body.setTexture(texture);

}

BombExploding::~BombExploding()
{
}

void BombExploding::Update(float start_time, float deltaTime, float switchTime)
{
	animation.Update(row, deltaTime, start_time);
	body.setTextureRect(animation.uvRect);
}

void BombExploding::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
