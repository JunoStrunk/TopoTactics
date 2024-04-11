#include "Vertex.h"

Vertex::Vertex(int id, float x, float y, VertexProps *props)
{
	this->id = id;
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
	draggable = false;
	hasPiece = false;
	isHappy = true;
}

void Vertex::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (hasPiece)
	{
		target.draw(piece, states);
	}
	else
	{
		target.draw(shape, states);
	}
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

sf::Color Vertex::getOutlineColor()
{
	return shape.getOutlineColor();
}

bool Vertex::isHovered()
{
	return hovered;
}

bool Vertex::isSelected() const
{
	return selected;
}

bool Vertex::isDraggable() const
{
	return draggable;
}

std::string Vertex::getPlayer()
{
	return playerIdentity;
}
bool Vertex::getIsHappy()
{
	return isHappy;
}
bool Vertex::getHasPiece()
{
	return hasPiece;
}

Coalition Vertex::getCoal()
{
	if (!hasPiece)
		return;
	return piece.getCoal();
}

/*======= Setters =============================*/

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

void Vertex::setDraggable(bool draggable)
{
	this->draggable = draggable;
}

void Vertex::setFillColor(sf::Color color)
{
	shape.setFillColor(color);
}

void Vertex::setOutlineColor(sf::Color color)
{
	shape.setOutlineColor(color);
}

void Vertex::setCoords(float x, float y)
{
	xpos = x;
	ypos = y;
	shape.setPosition(xpos, ypos);
}

void Vertex::setIsHappy(bool happy)
{
	isHappy = happy;
	if (hasPiece)
		piece.setTex(isHappy);
}

void Vertex::setHasPiece(bool hasPiece)
{
	this->hasPiece = hasPiece;
}
void Vertex::setPlayer(std::string identity)
{
	playerIdentity = identity;
}

void Vertex::setPiece(Piece piece)
{
	this->piece = piece;
	this->piece.setPosition(xpos - piece.getSprite().getGlobalBounds().width / 2.0, ypos - piece.getSprite().getGlobalBounds().height / 2.0);
	piece.setTex(isHappy);
	hasPiece = true;
}