#include "Board.h"

Board::~Board()
{
	for (const auto &pair : graph)
	{
		delete pair.second.first;
	}

	for (Piece *p : pieces)
		delete p;
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (Line line : edges)
		line.draw(target, states);

	// Printing the contents of the map
	for (const auto &pair : graph)
	{
		target.draw(*(pair.second.first));
	}

	for (Piece *p : drawPieces)
		target.draw(*p);
}

void Board::loadBoard(const char *boardPath, VertexProps &vertexProps)
{
	std::ifstream boardFile(boardPath);

	if (!boardFile.is_open())
	{
		std::cout << "file not opened" << std::endl;
		return;
	}

	std::vector<Vertex *> vertices;
	std::string line;
	std::string delimiter = ",";
	size_t pos = 0;
	std::string token;

	getline(boardFile, line);

	int numV = std::stoi(line);

	for (int i = 0; i < numV; i++)
	{
		getline(boardFile, line);

		// get vertex id
		pos = line.find(delimiter);
		token = line.substr(0, pos);
		int vID = std::stoi(token);
		line.erase(0, pos + delimiter.length());

		// get x position
		pos = line.find(delimiter);
		token = line.substr(0, pos);
		float xpos = std::stof(token);
		line.erase(0, pos + delimiter.length());

		// get y position
		float ypos = std::stof(line);

		// create vertex
		Vertex *vertex = new Vertex(vID, xpos, ypos, &vertexProps);
		vertices.push_back(vertex);
	}

	// create edges
	for (int i = 0; i < numV; i++)
	{
		getline(boardFile, line);

		// vertex id
		pos = line.find(delimiter);
		token = line.substr(0, pos);
		int vID = std::stoi(token);
		line.erase(0, pos + delimiter.length());

		// edges
		while ((pos = line.find(delimiter)) != std::string::npos)
		{
			token = line.substr(0, pos);
			int endpointID = std::stoi(token);
			// board.addConnection(board.getVertex(vID), board.getVertex(endpointID));

			for (int j = 0; j < vertices.size(); j++)
			{
				if (vertices[j]->getId() == endpointID)
					addConnection(vertices[i], vertices[j]);
			}

			line.erase(0, pos + delimiter.length());
		}

		int endpointID = std::stoi(line);
		// board.addConnection(board.getVertex(vID), board.getVertex(endpointID));

		for (int j = 0; j < vertices.size(); j++)
		{
			if (vertices[j]->getId() == endpointID)
				addConnection(vertices[i], vertices[j]);
		}
	}
}

// This function parse's opponent's move and places the correct sprite where it needs to go
void Board::recieveUpdate(int changeKey, sf::Color fillColor, std::string identity)
{

	Piece *p = nullptr;
	if (fillColor == sf::Color::Green)
		p = (identity == "Player 1") ? pieces[4] : pieces[0];
	else if (fillColor == sf::Color::Blue)
		p = (identity == "Player 1") ? pieces[5] : pieces[1];
	else if (fillColor == sf::Color{255, 192, 203}) // PINK
		p = (identity == "Player 1") ? pieces[6] : pieces[2];
	else if (fillColor == sf::Color{255, 165, 0}) // ORANGE
		p = (identity == "Player 1") ? pieces[7] : pieces[3];

	if (p != nullptr)
		getVertex(changeKey)->setPiece(*p);
}

std::vector<std::pair<Vertex *, sf::Color>> Board::updateBoard(int changeKey)
{
	std::vector<std::pair<Vertex *, sf::Color>> changedVertices;
	Vertex *v = getVertex(changeKey);

	// start by setting the selected vertex fill color
	if (v->getCoal() == GREEN)
		changedVertices.push_back(std::make_pair(v, sf::Color::Green));
	else if (v->getCoal() == BLUE)
		changedVertices.push_back(std::make_pair(v, sf::Color::Blue));
	else if (v->getCoal() == PINK)
		changedVertices.push_back(std::make_pair(v, sf::Color{255, 192, 203})); // PINK
	else if (v->getCoal() == ORANGE)
		changedVertices.push_back(std::make_pair(v, sf::Color{255, 165, 0})); // ORANGE

	updateNeighbors(changeKey, &changedVertices);

	return changedVertices;
}

void Board::updateNeighbors(int changeKey, std::vector<std::pair<Vertex *, sf::Color>> *changedVertices)
{
	Vertex *v = getVertex(changeKey);
	std::vector<Vertex *> *neighbors = getNeighbors(changeKey);
	for (int i = 0; i < getNeighbors(changeKey)->size(); i++)
	{
		if (v->getCoal() == GREEN)
		{
			// if connected vertex is blue, make connected vertex unhappy
			if ((*neighbors)[i]->getColor() == sf::Color::Blue)
			{
				changedVertices->push_back(std::make_pair((*neighbors)[i], sf::Color::Red));
				(*neighbors)[i]->setIsHappy(false);
			}
			// if connected vertex is orange, make selected vertex unhappy
			else if ((*neighbors)[i]->getColor() == sf::Color{255, 165, 0}) // ORANGE
			{
				changedVertices->push_back(std::make_pair(v, sf::Color::Red));
				v->setIsHappy(false);
			}
		}

		else if (v->getCoal() == BLUE)
		{
			// if connection is pink, make pink unhappy
			if ((*neighbors)[i]->getColor() == sf::Color{255, 192, 203}) // PINK
			{
				changedVertices->push_back(std::make_pair((*neighbors)[i], sf::Color::Red));
				(*neighbors)[i]->setIsHappy(false);
			}
			// if connection is green, make selected vertex unhappy
			else if ((*neighbors)[i]->getColor() == sf::Color::Green)
			{
				changedVertices->push_back(std::make_pair(v, sf::Color::Red));
				v->setIsHappy(false);
			}
		}
		else if (v->getCoal() == PINK)
		{
			// if connection is orange, make orange unhappy
			if ((*neighbors)[i]->getColor() == sf::Color{255, 165, 0}) // ORANGE
			{
				changedVertices->push_back(std::make_pair((*neighbors)[i], sf::Color::Red));
				(*neighbors)[i]->setIsHappy(false);
			}

			// if connection is blue, make selected vertex unhappy
			else if ((*neighbors)[i]->getColor() == sf::Color::Blue)
			{
				changedVertices->push_back(std::make_pair(v, sf::Color::Red));
				v->setIsHappy(false);
			}
		}
		else if (v->getCoal() == ORANGE)
		{
			// if connection is green, make green unhappy
			if ((*neighbors)[i]->getColor() == sf::Color::Green)
			{
				changedVertices->push_back(std::make_pair((*neighbors)[i], sf::Color::Red));
				(*neighbors)[i]->setIsHappy(false);
			}

			// if connection is pink, make selected vertex unhappy
			if ((*neighbors)[i]->getColor() == sf::Color{255, 192, 203})
			{
				changedVertices->push_back(std::make_pair(v, sf::Color::Red));
				v->setIsHappy(false);
			}
		}
	}
}

// GETTERS ================================================================================================================
Vertex *Board::getVertex(int key)
{
	return graph.at(key).first;
}

std::vector<Vertex *> *Board::getNeighbors(int key)
{
	return &(graph.at(key).second);
}

Vertex *Board::getNeighborAt(int key, int nkey)
{
	return graph.at(key).second[nkey];
}

bool *Board::getEditing()
{
	return &editing;
}

void Board::printIds()
{
	for (const auto &pair : graph)
	{
		std::cout << "Key: " << pair.first << " Vertex ID: " << pair.second.first->getId() << "\n";
	}
}
std::unordered_map<int, std::pair<Vertex *, std::vector<Vertex *>>> Board::getVertexGraph()
{
	return graph;
}

// SETTERS ================================================================================================================
void Board::setEditing(bool editing)
{
	this->editing = editing;
}

void Board::setHrzLayoutGrp(HrzLayoutGrp ps)
{
	drawPieces = ps.getItems();
}

bool Board::addVertex(Vertex *v)
{
	auto result = graph.try_emplace(v->getId());
	if (result.second)
		graph[v->getId()].first = v;
	return result.second;
}

void Board::addConnection(Vertex *from, Vertex *to)
{
	// TODO: Currently no way checking if one is connected to something twice
	// If from or to are not already in map, this will add them.
	graph[from->getId()].first = from;
	graph[from->getId()].second.push_back(to);
	graph[to->getId()].first = to;
	graph[to->getId()].second.push_back(from);

	Line line(from, to);
	edges.push_back(line);
}

void Board::addPiece(Piece *piece)
{
	pieces.push_back(piece);
}

std::unordered_map<int, std::pair<Vertex *, std::vector<Vertex *>>> Board::getConnection()
{
	return graph;
}

void Board::updateEdge(int id, float x, float y)
{
	for (Line line : edges)
	{
		if (line.getFromID() == id)
			line.setFromCoords(x, y);
		if (line.getToID() == id)
			line.setToCoords(x, y);
	}
}

// Move this to shader??
void Board::mouseMoved(sf::Event &event)
{
	for (auto &pair : graph)
	{
		if (pair.second.first->getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
			pair.second.first->setHovered(true);
		else
			pair.second.first->setHovered(false);
		if (pair.second.first->isDraggable())
		{
			pair.second.first->setCoords(event.mouseMove.x, event.mouseMove.y);
			updateEdge(pair.first, event.mouseMove.x, event.mouseMove.y);
		}
	}
}

Vertex *Board::mouseReleased(sf::Event &event)
{
	Vertex *selected = nullptr;
	for (auto &pair : graph)
	{
		if (!editing && pair.second.first->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
		{
			pair.second.first->setSelected(true);
			selected = pair.second.first;
		}
		else
		{
			pair.second.first->setDraggable(false);
		}
	}

	if (selected != nullptr)
		std::cout << "Clicked ID: " << selected->getId() << "\n";
	return selected;
}

void Board::mousePressed(sf::Event &event)
{
	for (auto &pair : graph)
	{
		if (editing && pair.second.first->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
		{
			pair.second.first->setDraggable(true);
		}
	}
}

Piece *Board::mouseClickPiece(sf::Event &event)
{
	Piece *selected = nullptr;
	for (Piece *piece : pieces)
	{

		if (piece->getSprite().getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
		{

			piece->setSelected(true);
			selected = piece;
			break;
		}

		// if (piece->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
		//{
		//	piece->setSelected(true);
		//	selected = piece;
		// }
	}
	return selected;
}