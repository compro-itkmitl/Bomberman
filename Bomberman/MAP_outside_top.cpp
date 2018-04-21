#include "MAP_outside_top.h"
#include <SFML\Graphics.hpp>

using namespace sf;

MAP_outside_top::MAP_outside_top(Texture* texture, Vector2f size, Vector2f position)
{
	body.setSize(size);
	body.setTexture(texture);
	body.setPosition(position);
}


MAP_outside_top::~MAP_outside_top()
{
}


void MAP_outside_top::Draw(RenderWindow& window)
{
	window.draw(body);
}