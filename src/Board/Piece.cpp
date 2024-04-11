#include "Piece.h"

Piece::Piece(Coalition coal, std::string player, sf::Texture &hapTex1, sf::Texture &sadTex1, sf::Texture &hapTex2, sf::Texture &sadTex2)
{
	this->coal = coal;
	placed = false;
	selected = false;
	if (player == "Player 1")
	{
		if (coal == GREEN)
		{
			this->width = 296;
			this->height = 340;
		}
		else if (coal == BLUE)
		{
			this->width = 430;
			this->height = 278;
		}
		else if (coal == PINK)
		{
			this->width = 330;
			this->height = 268;
		}
		else if (coal == ORANGE)
		{
			this->width = 250;
			this->height = 295;
		}
		this->hapTex = hapTex1;
		this->sadTex = sadTex1;
	}
	else
	{
		if (coal == GREEN)
		{
			this->width = 296;
			this->height = 340;
		}
		else if (coal == BLUE)
		{
			this->width = 430;
			this->height = 278;
		}
		else if (coal == PINK)
		{
			this->width = 330;
			this->height = 268;
		}
		else if (coal == ORANGE)
		{
			this->width = 250;
			this->height = 295;
		}
		this->hapTex = hapTex2;
		this->sadTex = sadTex2;
	}
	currSprite.setTexture(this->hapTex);
	currSprite.setScale(scale, scale);
}

// Piece &Piece::operator=(const Piece &p)
// {
// }

void Piece::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(currSprite, states);
}

/*======= Getters =============================*/

float Piece::getWidth()
{
	return width;
}

float Piece::getHeight()
{
	return height;
}

float Piece::getScale()
{
	return scale;
}

sf::FloatRect Piece::getGlobalBounds() const
{
	return currSprite.getGlobalBounds();
}

sf::Sprite Piece::getSprite()
{
	return currSprite;
}

Coalition Piece::getCoal()
{
	return coal;
}

/*======= Setters =============================*/
void Piece::setPosition(float x, float y)
{
	currSprite.setPosition(sf::Vector2f(x, y));
}
void Piece::setSelected(bool selected)
{
	this->selected = selected;
}

void Piece::setTex(bool isHappy)
{
	if (isHappy)
		currSprite.setTexture(hapTex);
	else
		currSprite.setTexture(sadTex);
}