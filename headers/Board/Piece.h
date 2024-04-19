#include <SFML/Graphics.hpp>
#include <iostream>
#include "Coalition.h"
#pragma once

class Piece : public sf::Drawable
{
private:
	bool isHappy; // default true
	Coalition coal;
	sf::Sprite sprite;
	sf::Texture texture;
	bool selected;
	float xpos;
	float ypos;

public:
	Piece(Coalition coal, std::string player);
	~Piece() = default;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	// Getters
	sf::FloatRect getGlobalBounds() const;
	sf::Sprite getSprite();
	Coalition getCoal();

	// Setters
	void setPosition(float x, float y);
	void setSelected(bool selected);
	void setCoal(Coalition coalition);
};
