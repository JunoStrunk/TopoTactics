#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Vertex.h"
#include "HrzLayoutGrp.h"
#include "Piece.h"
#include "Line.h"
#pragma once

class Board : public sf::Drawable
{
private:
	bool editing = false;
	std::unordered_map<int, std::pair<Vertex *, std::vector<Vertex *>>> graph; // Key: , Value pair<from, to vector>
	std::vector<Piece *> pieces;
	std::vector<Line> edges;
	std::vector<Piece *> drawPieces;

public:
	Board() = default;
	~Board();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void loadBoard(const char *boardPath, VertexProps &vertexProps);
	void recieveUpdate(int changeKey, sf::Color fillColor, std::string identity);
	std::vector<std::pair<Vertex *, sf::Color>> updateBoard(int changeKey);
	void updateNeighbors(int changeKey, std::vector<std::pair<Vertex *, sf::Color>> *changedVertices);

	// Getters
	Vertex *getVertex(int key);
	std::vector<Vertex *> *getNeighbors(int key);
	Vertex *getNeighborAt(int key, int nkey);
	bool *getEditing();
	void printIds();
	std::unordered_map<int, std::pair<Vertex *, std::vector<Vertex *>>> getVertexGraph();

	// Setters
	void setEditing(bool editing);
	void setHrzLayoutGrp(HrzLayoutGrp ps);
	bool addVertex(Vertex *v);
	void addConnection(Vertex *from, Vertex *to);
	void addPiece(Piece *piece);
	std::unordered_map<int, std::pair<Vertex *, std::vector<Vertex *>>> getConnection();
	void updateEdge(int id, float x, float y);

	// Events
	void mouseMoved(sf::Event &event);
	Vertex *mouseReleased(sf::Event &event);
	void mousePressed(sf::Event &event);
	Piece *mouseClickPiece(sf::Event &event, std::vector<Piece *> &pieces);
};
