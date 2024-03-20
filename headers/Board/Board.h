#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Vertex.h"
#include "Piece.h"
#pragma once

class Board : public sf::Drawable
{
private:
	bool editing;
	std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> graph; // Key: , Value pair<from, to vector>
	std::vector<sf::VertexArray> edges;
	std::vector<Piece*> pieces;

public:
	Board()
	{
		editing = false;
	};
	~Board();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void loadBoard(const char *boardPath, VertexProps &vertexProps);
	Vertex *getVertex(int key);
	bool *getEditing();
	void printIds();
	std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> getVertexGraph();

	void setEditing(bool editing);
	bool addVertex(Vertex *v);
	void addConnection(Vertex *from, Vertex *to);
	void addPiece(Piece *piece);
	std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> getConnection();

	void mouseMoved(sf::Event &event);
	Vertex *mouseReleased(sf::Event &event);
	void mousePressed(sf::Event &event);
	Piece *mouseClickPiece(sf::Event &event, std::vector<Piece*> &pieces);

};
