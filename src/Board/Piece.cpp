#include "Piece.h"

Piece::Piece(Coalition coal, std::string player)
{
	isHappy = true;
	this->coal = coal;
	selected = false;
	sprite.scale(4, 4);

	if (player == "Player 1")
	{
		if (coal == GREEN)
		{
			if (!texture.loadFromFile("../assets/p1_green.png"))
			{
				// Handle error if texture loading fails
			}
			sprite.setTexture(texture);
		}
		else if (coal == BLUE)
		{
			if (!texture.loadFromFile("../assets/p1_blue.png"))
			{
				// Handle error if texture loading fails
			}
			sprite.setTexture(texture);
		}
		else if (coal == PINK)
		{
			if (!texture.loadFromFile("../assets/p1_pink.png"))
			{
				// Handle error if texture loading fails
			}
			sprite.setTexture(texture);
		}
		else if (coal == ORANGE)
		{
			if (!texture.loadFromFile("../assets/p1_orange.png"))
			{
				// Handle error if texture loading fails
			}
			sprite.setTexture(texture);
		}
	}
	else
	{
		if (coal == GREEN)
		{
			if (!texture.loadFromFile("../assets/p2_green.png"))
			{
				// Handle error if texture loading fails
			}
			sprite.setTexture(texture);
		}
		else if (coal == BLUE)
		{
			if (!texture.loadFromFile("../assets/p2_blue.png"))
			{
				// Handle error if texture loading fails
			}
			sprite.setTexture(texture);
		}
		else if (coal == PINK)
		{
			if (!texture.loadFromFile("../assets/p2_pink.png"))
			{
				// Handle error if texture loading fails
			}
			sprite.setTexture(texture);
		}
		else if (coal == ORANGE)
		{
			if (!texture.loadFromFile("../assets/p2_orange.png"))
			{
				// Handle error if texture loading fails
			}
			sprite.setTexture(texture);
		}
	}
}

void Piece::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

/*======= Getters =============================*/

sf::FloatRect Piece::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

sf::Sprite Piece::getSprite()
{
	return sprite;
}

Coalition Piece::getCoal()
{
	return coal;
}

/*======= Setters =============================*/
void Piece::setPosition(float x, float y)
{
	sprite.setPosition(sf::Vector2f(x, y));
}
void Piece::setSelected(bool selected)
{
	this->selected = selected;
}