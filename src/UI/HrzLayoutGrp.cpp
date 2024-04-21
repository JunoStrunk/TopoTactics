#include "HrzLayoutGrp.h"

HrzLayoutGrp::HrzLayoutGrp(float xPos, float yPos, float padding)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->padding = padding;
}

void HrzLayoutGrp::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (Piece *p : items)
		target.draw(*p);
}

void HrzLayoutGrp::Add(Piece *piece)
{
	items.push_back(piece);
}

void HrzLayoutGrp::calculate()
{
	float runningPadding = xPos;
	for (int i = 0; i < items.size(); i++)
	{
		items[i]->setPosition(runningPadding, yPos);
		runningPadding += padding + items[i]->getWidth() * items[i]->getScale();
	}
}

std::vector<Piece *> HrzLayoutGrp::getItems()
{
	return items;
}