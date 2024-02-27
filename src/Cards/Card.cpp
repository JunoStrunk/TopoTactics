#include "Card.h"

// card constructor
Card::Card(float x, float y, CardProps *props)
{
	this->props = props;
	shape.setPosition(x, y);
	shape.setSize(sf::Vector2f(props->CARD_WIDTH, props->CARD_HEIGHT));
	shape.setFillColor(props->CARD_FILL_COLOR);
	shape.setOutlineThickness(props->CARD_OUTLINE_THICKNESS);
	shape.setOutlineColor(props->CARD_OUTLINE_COLOR);
	hovered = false;
	selected = false;
}

sf::RectangleShape Card::getShape()
{
	return shape;
}

bool Card::isHovered()
{
	return hovered;
}

void Card::setHovered(bool hover)
{
	hovered = hover;
}

void Card::setSelected(bool select)
{
	if (select)
	{
		shape.setOutlineColor(props->CARD_SELECTED_OUTLINE_COLOR);
		selected = true;
		// hovered = true;
	}
	else
	{
		shape.setOutlineColor(props->CARD_OUTLINE_COLOR);
		selected = false;
		// hovered = false;
	}
}

bool Card::isSelected()
{
	return selected;
}