#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Card.h"
#include "CardProps.h"
#include "VertexProps.h"
#include "Board.h"

int main()
{

	// window
	const sf::Color WINDOW_COLOR = sf::Color::White;
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Test window");

	CardProps cardProps;
	VertexProps vertexProps;
	Board board;

	// create nodes
	// for (int i = 0; i < vertexProps.NUM_NODES; i++)
	// {
	// 	float x = i * (vertexProps.NODE_SIZE + 100) + 100;
	// 	float y = vertexProps.NODE_SIZE + 10;

	// 	Vertex *v = new Vertex(x, y, &vertexProps);
	// 	board.addVertex(v);
	// }

	Vertex *v1 = new Vertex(1, 100, vertexProps.NODE_SIZE + 10, &vertexProps);
	Vertex *v2 = new Vertex(2, (vertexProps.NODE_SIZE + 100) + 100, vertexProps.NODE_SIZE + 10, &vertexProps);
	board.addConnection(v1, v2);
	Vertex *v3 = new Vertex(3, 2 * (vertexProps.NODE_SIZE + 100) + 100, vertexProps.NODE_SIZE + 10, &vertexProps);
	board.addConnection(v2, v3);

	// Start the game loop
	while (window.isOpen())
	{
		// Clear screen
		window.clear(WINDOW_COLOR);

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonReleased)
			{
				board.mouseClick(event);
			}
			if (event.type == sf::Event::MouseMoved)
			{
				board.mouseMoved(event);
			}
		}

		window.draw(board);

		// Update the window
		window.display();
	}

	delete v1;
	delete v2;
	delete v3;

	return EXIT_SUCCESS;
}