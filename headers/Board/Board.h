#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Vertex.h"
#include "Card.h"
#include "Piece.h"
#include "Line.h"
#pragma once

class Board : public sf::Drawable
{
private:
	bool editing = false;
	std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> graph; // Key: , Value pair<from, to vector>
	std::vector<Card *> cards;
	std::vector<Piece *> pieces;
	std::vector<Line> edges;

public:
	Board() = default;
	~Board();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void loadBoard(const char *boardPath, VertexProps &vertexProps);

	// Getters
	Vertex *getVertex(int key);
	bool *getEditing();
	void printIds();
	std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> getVertexGraph();

	// Setters
	void setEditing(bool editing);
	bool addVertex(Vertex *v);
	void addConnection(Vertex *from, Vertex *to);
	void addCard(Card *card);
	void addPiece(Piece *piece);
	std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> getConnection();
	void updateEdge(int id, float x, float y);

	// Events
	void mouseMoved(sf::Event &event);
	Vertex *mouseReleased(sf::Event &event);
	void mousePressed(sf::Event &event);
	Card *mouseClickCard(sf::Event &event, std::vector<Card *> &cards);
	Piece *mouseClickPiece(sf::Event &event, std::vector<Piece *> &pieces);
};
