#pragma once
#include <SFML\Graphics.hpp>
#include "Collider.h"

using namespace sf;

class MAP_GROUND
{
public:
	MAP_GROUND(Texture* texture1, Texture* texture2, Vector2f size, Vector2f position, int x, int y);
	~MAP_GROUND();
	
	void Draw(sf::RenderWindow& window);
	Collider GetCollider() { return Collider(body); }

private:
	RectangleShape body;
};

