#include "MAP_GROUND.h"

using namespace sf;

MAP_GROUND::MAP_GROUND(Texture* texture1, Texture* texture2, Vector2f size, Vector2f position, int x, int y)
{
	body.setSize(size);
	body.setOrigin(size / 2.0f);
	if((x+y)%2) body.setTexture(texture1);
	else		body.setTexture(texture2);
	body.setPosition(position);
}


MAP_GROUND::~MAP_GROUND()
{
}

void MAP_GROUND::Draw(RenderWindow& window)
{
	window.draw(body);
}

