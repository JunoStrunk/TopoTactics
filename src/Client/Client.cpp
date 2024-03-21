
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Button.h"
#include "Card.h"
#include "Piece.h"
#include "TextureManager.h"
#include <iostream>
#include <fstream>
#include <thread>
// client
const sf::IpAddress IP = "";
// const sf::IpAddress IP = sf::IpAddress::getLocalAddress();
const int PORT = 2222;

/*======= Serialization ========================*/
sf::Packet &operator<<(sf::Packet &packet, const Vertex &m);

void printMessage(std::string msg)
{
	std::cout << msg << std::endl;
}

class Client
{
	std::string identity;
	std::string playerTurn;
	Piece *selectedPiece;
	bool isPieceSelected;

public:
	Client()
	{
		selectedPiece = nullptr;
		isPieceSelected = false;
	};

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

	std::vector<std::pair<Vertex *, sf::Color>> setVertexColors(Piece *&p, Vertex *&v, Board &board)
	{
		std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> graph = board.getConnection();
		std::vector<std::pair<Vertex *, sf::Color>> changedVertices;
		std::vector<Vertex *> connections = graph[v->getId()].second;
		Coalition coal = p->getCoal();

		// start by setting the selected vertex fill color
		if (coal == GREEN)
			changedVertices.push_back(std::make_pair(v, sf::Color::Green));

		else if (coal == BLUE)
			changedVertices.push_back(std::make_pair(v, sf::Color::Blue));

		else if (coal == PINK)
			changedVertices.push_back(std::make_pair(v, sf::Color{255, 192, 203})); // PINK

		else if (coal == ORANGE)
			changedVertices.push_back(std::make_pair(v, sf::Color{255, 165, 0})); // ORANGE

		// next, set the connection outline color, if unhappy
		for (int i = 0; i < connections.size(); i++)
		{
			if (coal == GREEN)
			{
				// if connected vertex is blue, make connected vertex unhappy
				if (connections[i]->getColor() == sf::Color::Blue)
				{
					changedVertices.push_back(std::make_pair(connections[i], sf::Color::Red));
					connections[i]->setIsHappy(false);
				}

				// if connected vertex is orange, make selected vertex unhappy
				else if (connections[i]->getColor() == sf::Color{255, 165, 0}) // ORANGE
				{
					changedVertices.push_back(std::make_pair(v, sf::Color::Red));
					v->setIsHappy(false);
				}
			}

			else if (coal == BLUE)
			{
				// if connection is pink, make pink unhappy
				if (connections[i]->getColor() == sf::Color{255, 192, 203}) // PINK
				{
					changedVertices.push_back(std::make_pair(connections[i], sf::Color::Red));
					connections[i]->setIsHappy(false);
				}

				// if connection is green, make selected vertex unhappy
				else if (connections[i]->getColor() == sf::Color::Green)
				{
					changedVertices.push_back(std::make_pair(v, sf::Color::Red));
					v->setIsHappy(false);
				}
			}

			else if (coal == PINK)
			{
				// if connection is orange, make orange unhappy
				if (connections[i]->getColor() == sf::Color{255, 165, 0}) // ORANGE
				{
					changedVertices.push_back(std::make_pair(connections[i], sf::Color::Red));
					connections[i]->setIsHappy(false);
				}

				// if connection is blue, make selected vertex unhappy
				else if (connections[i]->getColor() == sf::Color::Blue)
				{
					changedVertices.push_back(std::make_pair(v, sf::Color::Red));
					v->setIsHappy(false);
				}
			}

			else if (coal == ORANGE)
			{
				// if connection is green, make green unhappy
				if (connections[i]->getColor() == sf::Color::Green)
				{
					changedVertices.push_back(std::make_pair(connections[i], sf::Color::Red));
					connections[i]->setIsHappy(false);
				}

				// if connection is pink, make selected vertex unhappy
				if (connections[i]->getColor() == sf::Color{255, 192, 203})
				{
					changedVertices.push_back(std::make_pair(v, sf::Color::Red));
					v->setIsHappy(false);
				}
			}
		}

		return changedVertices;
	}

	void sendNeighborColorPacket(std::vector<std::pair<Vertex *, sf::Color>> changedVetrices, sf::TcpSocket &socket)
	{
		// create packet to send
		sf::Packet packet;

		packet << static_cast<sf::Uint32>(changedVetrices.size());

		for (std::pair<Vertex *, sf::Color> vertex : changedVetrices)
		{
			packet << vertex.first->getId();
			packet << vertex.second.toInteger();
		}

		// packet << vertex->getId() << "red";

		if (socket.send(packet) != sf::Socket::Done)
		{
			std::cerr << "Error: Failed to send packet to the server" << std::endl;
			// handle error accordingly
		}
	}

	// void sendCircleColorPacket(Vertex *vertex, sf::TcpSocket &socket)
	// {

	// 	// create packet to send
	// 	sf::Packet packet;

	// 	if (vertex->getColor() == sf::Color::Blue)
	// 	{
	// 		std::cout << "Sending: " << vertex->getId() << "\n";
	// 		packet << vertex->getId() << "green";
	// 	}
	// 	else
	// 	{
	// 		packet << vertex->getId() << "blue";
	// 	}

	// 	// send packet to server
	// 	if (socket.send(packet) != sf::Socket::Done)
	// 	{
	// 		std::cerr << "Error: Failed to send packet to the server" << std::endl;
	// 		// handle error accordingly
	// 	}
	// }

	Piece *getSelectedPiece()
	{
		return selectedPiece;
	}

	bool hasSelectedPiece()
	{
		return isPieceSelected;
	}

	void setHasSelectedPiece(bool selected)
	{
		isPieceSelected = selected;
	}

	void setSelectedPiece(Piece *&piece)
	{
		selectedPiece = piece;
	}

	void calculateScore(Board &board)
	{
		std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> graph = board.getVertexGraph();
		int player1Score = 0;
		int player2Score = 0;

		for (auto i = graph.begin(); i != graph.end(); i++)
		{
			Vertex *v = i->second.first;

			if (v->getHasPiece())
			{
				std::cout << "Vertex Id:" << v->getId() << std::endl;
				std::cout << "Is Happy: " << v->getIsHappy() << std::endl;
				std::cout << "Player: " << v->getPlayer() << std::endl;
			}
			if (v->getHasPiece() == true && v->getIsHappy() == true && v->getPlayer() == "Player 1")
			{
				player1Score++;
			}
			else if (v->getHasPiece() == true && v->getIsHappy() == true && v->getPlayer() == "Player 2")
			{
				player2Score++;
			}
		}
		std::cout << "Current score: Player 1 = " << player1Score << ", Player 2 = " << player2Score << std::endl;
	}
};

void receiveMessages(sf::TcpSocket &socket, Board &board, Client &client)
{
	while (true)
	{
		sf::Packet receivedPacket;
		if (socket.receive(receivedPacket) != sf::Socket::Done)
		{
			// We may need to handle an error, for now we can just break the while loop.
			break;
		}

		int id;
		sf::Uint32 color;
		sf::Uint32 size;

		receivedPacket >> size;

		for (sf::Uint32 i = 0; i < size; i++)
		{
			if (i == 0)
			{
				receivedPacket >> id;
				receivedPacket >> color;
				sf::Color fillColor = sf::Color(color);
				board.getVertex(id)->setFillColor(fillColor);
			}

			else
			{
				receivedPacket >> id;
				receivedPacket >> color;
				sf::Color outlineColor = sf::Color(color);
				board.getVertex(id)->setOutlineColor(outlineColor);
			}
		}

		std::string playerTurn;
		receivedPacket >> playerTurn;
		client.setPlayerTurn(playerTurn);

		// calculate score
		std::map<int, std::pair<Vertex *, std::vector<Vertex *>>> graph = board.getVertexGraph();
		int player1Score = 0;
		int player2Score = 0;

		for (auto i = graph.begin(); i != graph.end(); i++)
		{
			Vertex *v = i->second.first;

			if (client.getIdentity() == "Player 1")
			{
				if (v->getColor() != sf::Color::White && v->getOutlineColor() != sf::Color::Red)
				{
					if (v->getPlayer() == "Player 1")
						player1Score++;
					else
						player2Score++;
				}
			}

			else
			{
				if (v->getColor() != sf::Color::White && v->getOutlineColor() != sf::Color::Red)
				{
					if (v->getPlayer() == "Player 2")
						player2Score++;
					else
						player1Score++;
				}
			}
		}
		std::cout << "Current score: Player 1 = " << player1Score << ", Player 2 = " << player2Score << std::endl;
	}
}

void loadPieces(Board &board, std::vector<Piece *> &pieces, Coalition &coalition, std::string player)
{
	Piece *green = new Piece(GREEN, player);
	green->setPosition(250.0, 500);
	board.addPiece(green);
	pieces.push_back(green);

	Piece *blue = new Piece(BLUE, player);
	blue->setPosition(300.0, 500);
	board.addPiece(blue);
	pieces.push_back(blue);

	Piece *pink = new Piece(PINK, player);
	pink->setPosition(350.0, 500);
	board.addPiece(pink);
	pieces.push_back(pink);

	Piece *orange = new Piece(ORANGE, player);
	orange->setPosition(400.0, 500);
	board.addPiece(orange);
	pieces.push_back(orange);
}

int main()
{
	// Window
	const sf::Color WINDOW_COLOR = sf::Color::White;
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	// Create client object and initialize with socket
	Client client;
	sf::TcpSocket socket;
	client.initailize(socket);

	// Setup window and initialize game elements
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), client.getIdentity());

	// Create Texture Manager
	TextureManager textureManager;
	textureManager.LoadTexture("UI/continue");
	textureManager.LoadTexture("UI/continue_clicked");

	// Create Board
	CardProps cardProps;
	VertexProps vertexProps;
	Coalition coalition;
	std::vector<Piece *> pieces;
	Board board;
	board.loadBoard("../files/board1.txt", vertexProps);

	// Create Button
	Button continueButton(&(textureManager.GetTexture("UI/continue")), &(textureManager.GetTexture("UI/continue_clicked")), sf::Vector2f(600.0f, 200.0f));
	std::function<void()> continueButtonTest = std::bind(printMessage, "ContinueButtonWorks!");
	continueButton.bindOnClick(continueButtonTest);

	loadPieces(board, pieces, coalition, client.getIdentity());

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

				// if (event.type == sf::Event::MouseButtonPressed)
				// {
				// 	continueButton.mousePressed(event);
				// }

				if (event.type == sf::Event::MouseButtonReleased && !client.hasSelectedPiece())
				{

					Piece *p = board.mouseClickPiece(event, pieces);

					if (p != nullptr)
					{

						client.setSelectedPiece(p);
						client.setHasSelectedPiece(true);
					}
				}

				if (event.type == sf::Event::MouseButtonReleased && client.hasSelectedPiece())
				{

					Piece *p = client.getSelectedPiece();

					Vertex *v = board.mouseReleased(event);

					if (v != nullptr)
					{
						v->setIsHappy(true);
						v->setHasPiece(true);
						v->setPlayer(client.getIdentity());

						std::vector<std::pair<Vertex *, sf::Color>> changedVetrices = client.setVertexColors(p, v, board);
						client.sendNeighborColorPacket(changedVetrices, socket);

						client.setHasSelectedPiece(false);
						// client.calculateScore(board);
						break;
					}
				}

				// if (event.type == sf::Event::MouseMoved)
				// {
				// 	board.mouseMoved(event);
				// }
			}
		}

		window.draw(board);

		window.display();
	}

	// Disconnect from the server
	socket.disconnect();
	receiveThread.join();

	return 0;
}
