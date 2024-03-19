#include <iostream>
#include <SFML/Graphics.hpp>
#pragma once

class Button : public sf::Drawable
{
private:
	sf::Vector2f coords;
	sf::Sprite unclicked;
	sf::Sprite clicked;
	sf::Sprite *currSprite;
	std::function<void()> onClick;

public:
	Button(sf::Texture *unclicked, sf::Texture *clicked, sf::Vector2f coords);
	~Button();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void bindOnClick(std::function<void()> &action);
	void mouseReleased(sf::Event &event);
	void mousePressed(sf::Event &event);
};
