#include "BOT1.h"
#include "Collider.h"
#include <SFML\Graphics.hpp>


BOT1::BOT1(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, float speed, sf::Vector2f pos) :
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


BOT1::~BOT1()
{
}


void BOT1::Update(float deltaTime, float switchTime)
{

	sf::Vector2f movement(0.0f, 0.0f);
	switchTime *= 9.5f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))	movement.x -= speed * deltaTime * switchTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))	movement.x += speed * deltaTime * switchTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))	movement.y -= speed * deltaTime * switchTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))	movement.y += speed * deltaTime * switchTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
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

void BOT1::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}