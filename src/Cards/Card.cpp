#include "Card.h"

// card constructor
Card::Card(int id, float x, float y/*, CardProps *props*/)
{
	/*this->props = props;
	shape.setPosition(x, y);
	shape.setSize(sf::Vector2f(props->CARD_WIDTH, props->CARD_HEIGHT));
	shape.setFillColor(props->CARD_FILL_COLOR);
	shape.setOutlineThickness(props->CARD_OUTLINE_THICKNESS);
	shape.setOutlineColor(props->CARD_OUTLINE_COLOR);*/
	hovered = false;
	selected = false;
	played = false;
	cardID = id;
	std::string path = std::string("../assets/card") + std::to_string(id) + std::string(".png");
	texture.loadFromFile(path);
	sprite.scale(0.125, 0.125);
	sprite.setTexture(texture);
	sprite.setPosition(sf::Vector2f(x, y));
}

void Card::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

sf::FloatRect Card::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

sf::RectangleShape Card::getShape()
{
	return shape;
}

sf::Sprite Card::getSprite()
{
	return sprite;
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

void Card::setPlayed(bool wasPlayed) {
	played = wasPlayed;
}

bool Card::wasPlayed()
{
	return played;
}

int Card::getID() {
	return cardID;
}