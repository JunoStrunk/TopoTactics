#include <iostream>
#include <SFML/Graphics.hpp>
#include "CardProps.h"
#pragma once

class Card : public sf::Drawable
{
	sf::RectangleShape shape;
	CardProps *props;
	bool hovered;
	bool selected;
	bool played;
	sf::Sprite sprite;
	sf::Texture texture;
	int cardID;

public:
	// card constructor
	Card(int id, float x, float y/*, CardProps* props*/);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::FloatRect getGlobalBounds() const;
	sf::RectangleShape getShape();
	sf::Sprite getSprite();
	bool isHovered();
	void setHovered(bool hover);
	void setSelected(bool select);
	bool isSelected();
	void setPlayed(bool wasPlayed);
	bool wasPlayed();
	int getID();
};
