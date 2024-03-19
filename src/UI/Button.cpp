#include "Button.h"

Button::Button(sf::Texture *unclicked, sf::Texture *clicked, sf::Vector2f coords)
{
	this->unclicked.setTexture(*unclicked);
	this->clicked.setTexture(*clicked);
	this->coords = coords;
	currSprite = &(this->unclicked);
	onClick = NULL;
}

Button::~Button()
{
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(*currSprite);
}

void Button::bindOnClick(std::function<void()> &action)
{
	this->onClick = action;
}

void Button::mouseReleased(sf::Event &event)
{
	currSprite = &unclicked;
	if (currSprite->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
	{
		if (onClick != NULL)
			onClick();
	}
}

void Button::mousePressed(sf::Event &event)
{
	if (currSprite->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
	{
		currSprite = &clicked;
	}
}