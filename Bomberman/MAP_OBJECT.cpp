#include "MAP_OBJECT.h"

MAP_OBJECT::MAP_OBJECT(Texture* texture, Vector2f size, Vector2f position)
{
	body.setSize(size);
	body.setOrigin(sf::Vector2f(25.0f, 25.0f));
	body.setTexture(texture);
	body.setPosition(position);
}



MAP_OBJECT::~MAP_OBJECT()
{
}


void MAP_OBJECT::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}