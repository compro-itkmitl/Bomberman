#include "Immortal.h"
#include <SFML\Graphics.hpp>
#include "Collider.h"
using namespace sf;

Immortal::Immortal(Texture* texture, Vector2f size, Vector2f position)
{
	body.setSize(size);
	body.setOrigin(sf::Vector2f(25.0f, 25.0f));
	body.setTexture(texture);
	body.setPosition(position);
}


Immortal::~Immortal()
{
}

void Immortal::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}