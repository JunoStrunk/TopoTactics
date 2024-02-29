#include "Board.h"

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (sf::VertexArray line : edges)
		target.draw(line);

	// Printing the contents of the map
	for (const auto &pair : graph)
	{
		target.draw(pair.second.first->getShape());
	}
}

Vertex *Board::getVertex(int key)
{
	return graph.at(key).first;
}

void Board::printIds()
{
	for (const auto &pair : graph)
	{
		std::cout << "Key: " << pair.first << " Vertex ID: " << pair.second.first->getId() << "\n";
	}
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

void Board::mouseMoved(sf::Event &event)
{
	for (auto &pair : graph)
	{
		if (pair.second.first->getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
			pair.second.first->setHovered(true);
		else
			pair.second.first->setHovered(false);
	}
}

Vertex *Board::mouseClick(sf::Event &event)
{
	Vertex *selected = nullptr;
	for (auto &pair : graph)
	{
		if (pair.second.first->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
		{
			pair.second.first->setSelected(true);
			selected = pair.second.first;
		}
	}
	if (selected != nullptr)
		std::cout << "Clicked ID: " << selected->getId() << "\n";
	return selected;
}