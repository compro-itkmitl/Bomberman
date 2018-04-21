#pragma once
#include <SFML\Graphics.hpp>
#include "Destroy_item_Anim.h"
class Destroy_item
{
public:
	Destroy_item(sf::Texture* texture, sf::Vector2f bomb_pos, sf::Vector2u imageCount, float switchTime);
	~Destroy_item();

	void Update(float start_time, float deltaTime, float switchTime);
	void Draw(sf::RenderWindow& window);

	sf::Vector2f GetPosition() { return body.getPosition(); }

private:
	sf::RectangleShape body;
	Destroy_item_Anim animation;
	unsigned int row;
	float speed;
};

