#include <iostream>
#include <vector>
#include "Piece.h"
#pragma once

class HrzLayoutGrp
{
private:
	std::vector<Piece *> items;
	float xPos;
	float yPos;
	float padding;

public:
	HrzLayoutGrp(float xpos, float ypos, float padding);
	void Add(Piece *piece);
	void calculate();
};
