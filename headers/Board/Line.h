#include <iostream>
#include <SFML/Graphics.hpp>
#include "Vertex.h"
#pragma once

class Line : public sf::Drawable
{
private:
	int fromID;
	int toID;
	sf::VertexArray line;

public:
	Line(Vertex *from, Vertex *to);
	~Line(){};
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	int getFromID() const;
	int getToID() const;

	void setFromCoords(float x, float y);
	void setToCoords(float x, float y);
};
