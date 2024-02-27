#include "Vertex.h"

Vertex::Vertex(int id, float x, float y, VertexProps *props)
{
	this->id = id;
	piece = nullptr;
	xpos = x;
	ypos = y;
	shape.setPosition(xpos, ypos);
	shape.setRadius(props->NODE_SIZE);
	shape.setFillColor(props->NODE_FILL_COLOR);
	shape.setOrigin(props->NODE_SIZE, props->NODE_SIZE);
	shape.setOutlineThickness(props->NODE_OUTLINE_THICKNESS);
	shape.setOutlineColor(props->NODE_OUTLINE_COLOR);
	this->props = props;
	hovered = false;
	selected = false;
}

void Vertex::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(shape, states);
}

/*======= Getters =============================*/
int Vertex::getId() const
{
	return id;
}

float Vertex::getX()
{
	return xpos;
}

float Vertex::getY()
{
	return ypos;
}

sf::FloatRect Vertex::getGlobalBounds() const
{
	return shape.getGlobalBounds();
}

sf::CircleShape Vertex::getShape()
{
	return shape;
}

sf::Color Vertex::getColor()
{
	return shape.getFillColor();
}

bool Vertex::isHovered()
{
	return hovered;
}

bool Vertex::isSelected() const
{
	return selected;
}

/*======= Setters =============================*/

void Vertex::setFillColor(sf::Color color)
{
	shape.setFillColor(color);
}

void Vertex::setOutlineColor(sf::Color color)
{
	shape.setOutlineColor(color);
}

void Vertex::setHovered(bool hover)
{
	if (hover)
	{
		hover = true;
		setOutlineColor(props->NODE_OUTLINE_SELECTED_COLOR);
	}
	else
	{
		hover = false;
		setOutlineColor(props->NODE_OUTLINE_COLOR);
	}
}

void Vertex::setSelected(bool select)
{
	// if (select)
	// {
	// 	selected = true;
	// 	setFillColor(props->NODE_FILL_HOVER_COLOR);
	// }
}
