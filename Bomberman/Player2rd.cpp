#include "Player2rd.h"
#include "Collider.h"
#include <SFML\Graphics.hpp>


Player2rd::Player2rd(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, float speed, sf::Vector2f pos) :
	animation(texture, imageCount, switchTime)
{
	this->speed = speed;
	row = 0;
	face = 2;

	body.setSize(sf::Vector2f(50.0f, 75.0f));
	body.setOrigin(25.0f, 57.0f);
	body.setPosition(pos);
	//body.setFillColor(sf::Color::Red);
	body.setTexture(texture);
}

Player2rd::~Player2rd()
{
}


void Player2rd::Update(float deltaTime, float switchTime, int speed)
{
	this->speed = (float)speed;
	sf::Vector2f movement(0.0f, 0.0f);
	switchTime *= 9.5f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	movement.x -= speed * deltaTime * switchTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	movement.x += speed * deltaTime * switchTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))	movement.y -= speed * deltaTime * switchTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	movement.y += speed * deltaTime * switchTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
	{
		planting = 1;
	}

	if (movement.x == 0.0f && movement.y == 0.0f)
	{
		face = 99;
	}
	else
	{
		if (movement.x < 0.0f) {
			face = row = 0;
		}
		if (movement.x > 0.0f) {
			face = row = 1;
		}
		if (movement.y > 0.0f) {
			face = row = 2;
		}
		if (movement.y < 0.0f) {
			face = row = 3;
		}
	}

	animation.Update(row, deltaTime, face);
	body.setTextureRect(animation.uvRect);
	body.move(movement);

}

void Player2rd::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}