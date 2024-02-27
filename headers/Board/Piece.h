#include <iostream>
#include "Coalition.h"
#pragma once

class Piece
{
private:
	bool isHappy; // default true
	Coalition coal;

public:
	Piece(Coalition coal);
	~Piece(){};
};
