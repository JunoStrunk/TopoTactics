#include <iostream>
#include <vector>
#include "Piece.h"
#pragma once

class HrzLayoutGrp : public sf::Drawable
{
private:
	std::vector<Piece *> items;
	float xPos;
	float yPos;
	float padding;

public:
	HrzLayoutGrp(float xpos, float ypos, float padding);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void Add(Piece *piece);
	void calculate();
	std::vector<Piece *> getItems();
};
