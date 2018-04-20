#pragma once
#include <SFML\Graphics.hpp>
#include "Collider.h"
using namespace sf;
class MAP_OBJECT
{
public:

	MAP_OBJECT(Texture* texture, Vector2f size, Vector2f position);
	~MAP_OBJECT();

	void Draw(sf::RenderWindow& window);
	Collider  GetCollider() { return Collider(body); }

private:

	sf::RectangleShape body;

};

