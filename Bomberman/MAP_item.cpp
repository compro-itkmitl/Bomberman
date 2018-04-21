#include "MAP_item.h"
#include <SFML\Graphics.hpp>

using namespace sf;

MAP_item::MAP_item(Texture* texture, Vector2f size, Vector2f position)
{
	body.setSize(size);
	body.setOrigin(sf::Vector2f(20.0f, 35.0f));
	body.setTexture(texture);
	body.setPosition(position);
}

MAP_item::~MAP_item()
{
}

void MAP_item::Draw(RenderWindow& window)
{
	window.draw(body);
}