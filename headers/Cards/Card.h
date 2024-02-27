#include <iostream>
#include <SFML/Graphics.hpp>
#include "CardProps.h"
#pragma once

class Card
{
	sf::RectangleShape shape;
	CardProps *props;
	bool hovered;
	bool selected;

public:
	// card constructor
	Card(float x, float y, CardProps* props);

	sf::RectangleShape getShape();
	bool isHovered();
	void setHovered(bool hover);
	void setSelected(bool select);
	bool isSelected();
};
