
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Card.h"
#include <iostream>
#include <thread>
// client
const sf::IpAddress IP = "192.168.50.223";
// const sf::IpAddress IP = sf::IpAddress::getLocalAddress();
const int PORT = 2222;

/*======= Serialization ========================*/
sf::Packet &operator<<(sf::Packet &packet, const Vertex &m);

class Client
{
	std::string identity;
	std::string playerTurn;

public:
	Client(){};

	void initailize(sf::TcpSocket &socket)
	{

		// set server IP address and port number
		sf::IpAddress ip = IP;
		int port = PORT;

		// Connect to the server
		if (socket.connect(ip, port) != sf::Socket::Done)
		{
			std::cerr << "Error Failed to connect to the server" << std::endl;
			// handle error
		}

		// Receive identity from the server
		sf::Packet identityPacket;
		if (socket.receive(identityPacket) != sf::Socket::Done)
		{
			std::cerr << "Error: Failed to receive identity from the server" << std::endl;
			// handle error
		}

		identityPacket >> identity >> playerTurn;
		std::cout << "You are " << identity << std::endl;
		std::cout << "player turn: " << playerTurn << std::endl;
	}

	std::string getIdentity()
	{
		return identity;
	}

	std::string getPlayerTurn()
	{
		return playerTurn;
	}

	void setPlayerTurn(std::string turn)
	{
		playerTurn = turn;
	}

	void sendCircleColorPacket(Vertex *vertex, sf::TcpSocket &socket)
	{

		// create packet to send
		sf::Packet packet;

		// Create a message packet and send it to the server
		// We can use the Packet class to build blocks of data to transfer over the network
		// we can send a node position, for example position 10
		// and a coalition, for example green
		// packet << 10 << "green";

		if (vertex->getColor() == sf::Color::Blue)
		{
			std::cout << "Sending: " << vertex->getId() << "\n";
			packet << vertex->getId() << "green";
		}
		else
		{
			packet << vertex->getId() << "blue";
		}

		// send packet to server
		if (socket.send(packet) != sf::Socket::Done)
		{
			std::cerr << "Error: Failed to send packet to the server" << std::endl;
			// handle error accordingly
		}
	}
};

void receiveMessages(sf::TcpSocket &socket, Board &board, Client &client)
{
	while (true)
	{
		sf::Packet receivedPacket;
		if (socket.receive(receivedPacket) == sf::Socket::Done)
		{

			// int nodeNum;
			// std::string color;
			int id;
			std::string color;
			std::string playerTurn;
			receivedPacket >> id >> color >> playerTurn;

			std::cout << "id " << id << " color: " << color << " player turn: " << playerTurn << std::endl;
			client.setPlayerTurn(playerTurn);

			sf::Color fillColor;

			if (color == "blue")
			{

				fillColor = sf::Color::Blue;
				// board.getVertex(id)->setFillColor(fillColor);
			}
			else
			{
				fillColor = sf::Color::Green;
				// board.getVertex(id)->setFillColor(fillColor);
			}

			board.getVertex(id)->setFillColor(fillColor);

			// deselect node
			// board.getVertex(id)->setSelected(false);
			// board.getVertex(id)->setHovered(false);
		}
	}
}

int main()
{
	// window
	const sf::Color WINDOW_COLOR = sf::Color::White;
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	// create client object and initialize with socket
	Client client;
	sf::TcpSocket socket;
	client.initailize(socket);

	// setup window and initialize game elements
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TopoTactics!");

	// create Board
	CardProps cardProps;
	VertexProps vertexProps;
	Board board;

	Vertex *v1 = new Vertex(1, 100, vertexProps.NODE_SIZE + 10, &vertexProps);
	Vertex *v2 = new Vertex(2, (vertexProps.NODE_SIZE + 100) + 100, vertexProps.NODE_SIZE + 10, &vertexProps);
	board.addConnection(v1, v2);
	Vertex *v3 = new Vertex(3, 2 * (vertexProps.NODE_SIZE + 100) + 100, vertexProps.NODE_SIZE + 10, &vertexProps);
	board.addConnection(v2, v3);

	// Start the message receiving thread
	std::thread receiveThread(receiveMessages, std::ref(socket), std::ref(board), std::ref(client));

	// Game loop
	while (window.isOpen())
	{
		window.clear(WINDOW_COLOR);
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();

			// select game board
			// while (!input.isBoardSelected())
			// {
			// 	// TODO: pass .txt file to input, then render board from file???
			// 	// rendering.clear() will delete all game elements if needed before starting a new game
			// 	input.selectBoard();
			// }

			// check if player's turn
			if (client.getIdentity() == client.getPlayerTurn())
			{
				if (event.type == sf::Event::MouseButtonReleased)
				{
					Vertex *v = board.mouseClick(event);
					if (v != nullptr)
						client.sendCircleColorPacket(v, socket);
				}
				if (event.type == sf::Event::MouseMoved)
				{
					board.mouseMoved(event);
				}
			}
		}

		window.draw(board);

		window.display();
	}

	// Disconnect from the server
	receiveThread.join();
	socket.disconnect();

	return 0;
}