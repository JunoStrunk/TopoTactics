#include <iostream>
#include <SFML/Graphics.hpp>
#pragma once

struct VertexProps
{
public:
	const float NODE_SIZE = 15;
	const int NODE_OUTLINE_THICKNESS = 3;
	const sf::Color NODE_FILL_COLOR = sf::Color::White;
	const sf::Color NODE_OUTLINE_UNHAPPY_COLOR = sf::Color::Red;
	const sf::Color NODE_FILL_HOVER_COLOR = sf::Color::Blue;
	const sf::Color NODE_OUTLINE_COLOR = sf::Color::Black;
	const sf::Color NODE_OUTLINE_SELECTED_COLOR = sf::Color::Magenta;
};