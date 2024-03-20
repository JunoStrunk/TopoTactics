#include "Board.h"

Board::~Board()
{
	for (const auto &pair : graph)
	{
		delete pair.second.first;
	}
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (sf::VertexArray line : edges)
		target.draw(line);

	// Printing the contents of the map
	for (const auto &pair : graph)
	{
		target.draw(pair.second.first->getShape());
	}

	for (Piece* piece : pieces) {
		target.draw(piece->getSprite());
	}
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

Vertex *Board::getVertex(int key)
{
	return graph.at(key).first;
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
std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> Board::getVertexGraph()
{
	return graph;
}

void Board::setEditing(bool editing)
{
	this->editing = editing;
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

	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(from->getX(), from->getY());
	line[1].position = sf::Vector2f(to->getX(), to->getY());
	line[0].color = sf::Color::Black;
	line[1].color = sf::Color::Black;
	edges.push_back(line);
}

void Board::addPiece(Piece *piece) {
	pieces.push_back(piece);
}

std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> Board::getConnection() {
	return graph;
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

Piece *Board::mouseClickPiece(sf::Event &event, std::vector<Piece*> &pieces) 
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
		
		//if (piece->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) 
		//{
		//	piece->setSelected(true);
		//	selected = piece;
		//}
	}
	return selected;
}