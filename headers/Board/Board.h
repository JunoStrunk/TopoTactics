#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Vertex.h"
#pragma once

class Board : public sf::Drawable
{
private:
	std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> graph; // Key: , Value pair<from, to vector>
	std::vector<sf::VertexArray> edges;

public:
	Board(){};
	~Board();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void loadBoard(const char *boardPath, VertexProps &vertexProps);
	Vertex *getVertex(int key);
	void printIds();

	bool addVertex(Vertex *v);
	void addConnection(Vertex *from, Vertex *to);

	void mouseMoved(sf::Event &event);
	Vertex *mouseClick(sf::Event &event);
};
