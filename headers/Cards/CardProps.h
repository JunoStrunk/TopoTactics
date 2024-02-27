#include <iostream>
#include <SFML/Graphics.hpp>
#pragma once

struct CardProps
{
public:
	const int CARD_WIDTH = 60;
	const int CARD_HEIGHT = 90;
	const int NUM_CARDS = 5;
	const int CARD_OUTLINE_THICKNESS = 5;
	const sf::Color CARD_FILL_COLOR = sf::Color::Transparent;
	const sf::Color CARD_OUTLINE_COLOR = sf::Color::White;
	const sf::Color CARD_SELECTED_OUTLINE_COLOR = sf::Color::Yellow;
};