#include "BOT2.h"
#include "Collider.h"
#include <SFML\Graphics.hpp>


BOT2::BOT2(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, float speed, sf::Vector2f pos) :
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


BOT2::~BOT2()
{
}


void BOT2::Update(float deltaTime, float switchTime, int speed, sf::Vector2f curpos, sf::Vector2f despos)
{
	sf::Vector2f movement(0.0f, 0.0f);
	switchTime *= 9.5f;
	float botspeed = 0.75f;
	float xx = abs(curpos.x - despos.x);
	float yy = abs(curpos.y - despos.y);
	float dis = 1.0f;
	if (curpos.x > despos.x && xx > dis)	movement.x -= speed * deltaTime * switchTime * botspeed;
	if (curpos.x < despos.x && xx > dis)	movement.x += speed * deltaTime * switchTime * botspeed;
	if (curpos.y > despos.y && yy > dis)	movement.y -= speed * deltaTime * switchTime * botspeed;
	if (curpos.y < despos.y && yy > dis)	movement.y += speed * deltaTime * switchTime * botspeed;


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

void BOT2::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}