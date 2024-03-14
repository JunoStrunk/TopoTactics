#include <iostream>
#include <SFML/Graphics.hpp>
#pragma once

class Button
{
private:
	sf::Vector2f coords;
	sf::Sprite unclicked;
	sf::Sprite clicked;
	sf::Sprite *currSprite;

public:
	Button(sf::Texture *unclicked, sf::Texture *clicked, sf::Vector2f coords);
	~Button();
};
