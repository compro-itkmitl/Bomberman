#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class MAP_outside_top
{
public:
	MAP_outside_top(Texture* texture, Vector2f size, Vector2f position);
	~MAP_outside_top();

	void Draw(RenderWindow& window);

private:
	RectangleShape body;
};

