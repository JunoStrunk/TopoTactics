#include <SFML/Graphics.hpp>
#include <iostream>
#include "Coalition.h"
#include "TextureManager.h"
#pragma once

class Piece : public sf::Drawable
{
private:
	Coalition coal;
	sf::Sprite currSprite;

	// Textures
	sf::Texture hapTex;
	sf::Texture sadTex;

	bool selected;
	std::string identity;

	// Transformation
	float xpos;
	float ypos;
	float width;
	float height;
	float scale = 0.3;

public:
	Piece(Coalition coal, std::string player, sf::Texture &hapTex, sf::Texture &sadTex);
	Piece() = default;
	// Piece &operator=(const Piece &p);

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	// Getters
	float getWidth();
	float getHeight();
	float getScale();
	sf::FloatRect getGlobalBounds() const;
	sf::Sprite getSprite();
	Coalition getCoal();

	// Setters
	void setPosition(float x, float y);
	void setSelected(bool selected);
	void setTex(bool isHappy);
	void switchSides();
};
