#pragma once
#include <SFML\Graphics.hpp>
#include "Collider.h"
using namespace sf;

class MAP_item
{
public:
	MAP_item(Texture* texture, Vector2f size, Vector2f position);
	~MAP_item();

	void Draw(RenderWindow& window);
	Collider  GetCollider() { return Collider(body); }

private:
	RectangleShape body;
};

