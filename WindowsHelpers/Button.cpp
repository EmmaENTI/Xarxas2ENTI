#include "Button.h"

Button::Button(float x, float y, std::string texturePath)
{
	if (!texture.loadFromFile(texturePath))
	{
		//Manage Error
	}

	this->setPosition(x,y);
	this->setTexture(texture);
}

bool Button::CheckBound(unsigned int x, unsigned int y)
{
	return this->getGlobalBounds().contains(x,y);
}
