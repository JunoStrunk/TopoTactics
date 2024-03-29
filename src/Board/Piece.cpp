#include "Piece.h"

Piece::Piece(Coalition coal, std::string player)
{
	isHappy = true;
	this->coal = coal;
	selected = false;

	if (player == "Player 1")
	{
		if (coal == GREEN) // 292 333
		{
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(0, 0, 292, 333)))
			{
				// Handle error if texture loading fails
			}
			happySprite.setTexture(texture);
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(0, 333, 292, 333)))
			{
				// Handle error if texture loading fails
			}
			sadSprite.setTexture(texture);
			currSprite = &happySprite;
		}
		else if (coal == BLUE) // 424 333
		{
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(292, 0, 424, 333)))
			{
				// Handle error if texture loading fails
			}
			happySprite.setTexture(texture);
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(292, 333, 424, 333)))
			{
				// Handle error if texture loading fails
			}
			sadSprite.setTexture(texture);
			currSprite = &happySprite;
		}
		else if (coal == PINK) // 322 333
		{
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(716, 0, 322, 333)))
			{
				// Handle error if texture loading fails
			}
			happySprite.setTexture(texture);
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(716, 333, 322, 333)))
			{
				// Handle error if texture loading fails
			}
			sadSprite.setTexture(texture);
			currSprite = &happySprite;
		}
		else if (coal == ORANGE) // 242 333
		{
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(1038, 0, 242, 333)))
			{
				// Handle error if texture loading fails
			}
			happySprite.setTexture(texture);
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(1038, 333, 242, 333)))
			{
				// Handle error if texture loading fails
			}
			sadSprite.setTexture(texture);
			currSprite = &happySprite;
		}
		(*currSprite).setScale(.5f, .5f);
	}
	else
	{
		if (coal == GREEN) // 292 333
		{
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(0, 666, 292, 333)))
			{
				// Handle error if texture loading fails
			}
			happySprite.setTexture(texture);
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(0, 999, 292, 333)))
			{
				// Handle error if texture loading fails
			}
			sadSprite.setTexture(texture);
			currSprite = &happySprite;
		}
		else if (coal == BLUE) // 424 333
		{
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(292, 666, 424, 333)))
			{
				// Handle error if texture loading fails
			}
			happySprite.setTexture(texture);
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(292, 999, 424, 333)))
			{
				// Handle error if texture loading fails
			}
			sadSprite.setTexture(texture);
			currSprite = &happySprite;
		}
		else if (coal == PINK) // 322 333
		{
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(716, 666, 322, 333)))
			{
				// Handle error if texture loading fails
			}
			happySprite.setTexture(texture);
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(716, 999, 322, 333)))
			{
				// Handle error if texture loading fails
			}
			sadSprite.setTexture(texture);
			currSprite = &happySprite;
		}
		else if (coal == ORANGE) // 242 333
		{
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(1038, 666, 242, 333)))
			{
				// Handle error if texture loading fails
			}
			happySprite.setTexture(texture);
			if (!texture.loadFromFile("../assets/Pieces.png", sf::IntRect(1038, 999, 242, 333)))
			{
				// Handle error if texture loading fails
			}
			sadSprite.setTexture(texture);
			currSprite = &happySprite;
		}
		(*currSprite).setScale(.5f, .5f);
	}
}

void Piece::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw((*currSprite), states);
}

/*======= Getters =============================*/

sf::FloatRect Piece::getGlobalBounds() const
{
	return (*currSprite).getGlobalBounds();
}

sf::Sprite Piece::getSprite()
{
	return *currSprite;
}

Coalition Piece::getCoal()
{
	return coal;
}

/*======= Setters =============================*/
void Piece::setPosition(float x, float y)
{
	(*currSprite).setPosition(sf::Vector2f(x, y));
}
void Piece::setSelected(bool selected)
{
	this->selected = selected;
}