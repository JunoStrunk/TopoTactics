#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Piece.h"
#include "VertexProps.h"
#pragma once

class Vertex : public sf::Drawable
{
	int id;
	Piece *piece;
	VertexProps *props;
	sf::CircleShape shape;
	bool hovered;
	bool selected;
	bool draggable;
	float xpos;
	float ypos;

public:
	// Vertex constructor
	Vertex(int id, float x, float y, VertexProps *props);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	// Serialization

	// Getters
	int getId() const;
	float getX();
	float getY();
	sf::FloatRect getGlobalBounds() const;
	sf::CircleShape getShape();
	sf::Color getColor();
	bool isHovered();
	bool isSelected() const;
	bool isDraggable() const;

	// Setters
	void setHovered(bool hover);
	void setSelected(bool select);
	void setDraggable(bool draggable);
	void setFillColor(sf::Color color);
	void setOutlineColor(sf::Color color);
	void setCoords(float x, float y);
};
