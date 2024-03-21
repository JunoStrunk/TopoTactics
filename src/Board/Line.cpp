#include "Line.h"

Line::Line(Vertex *from, Vertex *to)
{
	fromID = from->getId();
	toID = to->getId();
	line.setPrimitiveType(sf::Lines);
	line.resize(2);

	line[0].position = sf::Vector2f(from->getX(), from->getY());
	line[1].position = sf::Vector2f(to->getX(), to->getY());
	line[0].color = sf::Color::Black;
	line[1].color = sf::Color::Black;
}

void Line::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(line);
}

int Line::getFromID() const
{
	return fromID;
}

int Line::getToID() const
{
	return toID;
}

void Line::setFromCoords(float x, float y)
{
	line[0].position = sf::Vector2f(x, y);
}

void Line::setToCoords(float x, float y)
{
	line[1].position = sf::Vector2f(x, y);
}