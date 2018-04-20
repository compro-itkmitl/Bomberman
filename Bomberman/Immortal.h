#pragma once
#include <SFML\Graphics.hpp>
#include "Collider.h"
using namespace sf;
class Immortal
{
public:
	Immortal(Texture* texture, Vector2f size, Vector2f position);
	~Immortal();

	void Draw(sf::RenderWindow& window);
	Collider  GetCollider() { return Collider(body); }

private:
	sf::RectangleShape body;
};

