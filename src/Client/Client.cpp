
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Button.h"
#include "Card.h"
#include "Piece.h"
#include "TextureManager.h"
#include "HrzLayoutGrp.h"
#include <iostream>
#include <fstream>
#include <thread>
// client
const sf::IpAddress IP = "";
// const sf::IpAddress IP = sf::IpAddress::getLocalAddress();
const int PORT = 2222;

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

	// Duplicate Code, Reappears in recieveMessages below
	// void calculateScore(Board &board)
	// {
	// 	std::unordered_map<int, std::pair<Vertex *, std::vector<Vertex *>>> graph = board.getVertexGraph();
	// 	int player1Score = 0;
	// 	int player2Score = 0;

	// 	for (auto i = graph.begin(); i != graph.end(); i++)
	// 	{
	// 		Vertex *v = i->second.first;

	// 		if (v->getHasPiece())
	// 		{
	// 			std::cout << "Vertex Id:" << v->getId() << std::endl;
	// 			std::cout << "Is Happy: " << v->getIsHappy() << std::endl;
	// 			std::cout << "Player: " << v->getPlayer() << std::endl;
	// 		}
	// 		if (v->getHasPiece() == true && v->getIsHappy() == true && v->getPlayer() == "Player 1")
	// 		{
	// 			player1Score++;
	// 		}
	// 		else if (v->getHasPiece() == true && v->getIsHappy() == true && v->getPlayer() == "Player 2")
	// 		{
	// 			player2Score++;
	// 		}
	// 	}
	// 	std::cout << "Current score: Player 1 = " << player1Score << ", Player 2 = " << player2Score << std::endl;
	// }
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
				if (board.getVertex(id)->getPlayer() != client.getIdentity())
					board.recieveUpdate(id, fillColor, client.getIdentity());
			}
			else
			{
				receivedPacket >> id;
				receivedPacket >> color;
				sf::Color outlineColor = sf::Color(color);
				board.getVertex(id)->setOutlineColor(outlineColor);
				if (outlineColor == sf::Color::Red)
					board.getVertex(id)->setIsHappy(false);
				else
					board.getVertex(id)->setIsHappy(true);
			}
		}

		std::string playerTurn;
		receivedPacket >> playerTurn;
		client.setPlayerTurn(playerTurn);

		// calculate score
		std::unordered_map<int, std::pair<Vertex *, std::vector<Vertex *>>> graph = board.getVertexGraph();
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
	TextureManager::LoadTexture("Dad1Sad", "CharactersHighlights", sf::IntRect(0, 0, 336, 376));
	TextureManager::LoadTexture("Dad1Happy", "CharactersHighlights", sf::IntRect(0, 424, 336, 376));
	TextureManager::LoadTexture("Mom1Sad", "CharactersHighlights", sf::IntRect(472, 0, 464, 308));
	TextureManager::LoadTexture("Mom1Happy", "CharactersHighlights", sf::IntRect(472, 424, 464, 308));
	TextureManager::LoadTexture("MH1Sad", "CharactersHighlights", sf::IntRect(944, 0, 360, 304));
	TextureManager::LoadTexture("MH1Happy", "CharactersHighlights", sf::IntRect(944, 424, 360, 304));
	TextureManager::LoadTexture("Bro1Sad", "CharactersHighlights", sf::IntRect(1416, 0, 284, 332));
	TextureManager::LoadTexture("Bro1Happy", "CharactersHighlights", sf::IntRect(1416, 424, 284, 332));
	TextureManager::LoadTexture("Dad2Sad", "CharactersHighlights", sf::IntRect(0, 848, 336, 376));
	TextureManager::LoadTexture("Dad2Happy", "CharactersHighlights", sf::IntRect(0, 1272, 336, 376));
	TextureManager::LoadTexture("Mom2Sad", "CharactersHighlights", sf::IntRect(472, 848, 464, 308));
	TextureManager::LoadTexture("Mom2Happy", "CharactersHighlights", sf::IntRect(472, 1272, 464, 308));
	TextureManager::LoadTexture("MH2Sad", "CharactersHighlights", sf::IntRect(944, 848, 360, 304));
	TextureManager::LoadTexture("MH2Happy", "CharactersHighlights", sf::IntRect(944, 1272, 360, 304));
	TextureManager::LoadTexture("Bro2Sad", "CharactersHighlights", sf::IntRect(1416, 848, 284, 332));
	TextureManager::LoadTexture("Bro2Happy", "CharactersHighlights", sf::IntRect(1416, 1272, 284, 332));

	HrzLayoutGrp pieceGroup(50, 350, 2);

	// Declare Pieces
	Piece *green1 = new Piece(GREEN, "Player 1", TextureManager::GetTexture("Dad1Happy"), TextureManager::GetTexture("Dad1Sad"));
	Piece *green2 = new Piece(GREEN, "Player 2", TextureManager::GetTexture("Dad2Happy"), TextureManager::GetTexture("Dad2Sad"));
	Piece *blue1 = new Piece(BLUE, "Player 1", TextureManager::GetTexture("Mom1Happy"), TextureManager::GetTexture("Mom1Sad"));
	Piece *blue2 = new Piece(BLUE, "Player 2", TextureManager::GetTexture("Mom2Happy"), TextureManager::GetTexture("Mom2Sad"));
	Piece *pink1 = new Piece(PINK, "Player 1", TextureManager::GetTexture("MH1Happy"), TextureManager::GetTexture("MH1Sad"));
	Piece *pink2 = new Piece(PINK, "Player 2", TextureManager::GetTexture("MH2Happy"), TextureManager::GetTexture("MH2Sad"));
	Piece *orange1 = new Piece(ORANGE, "Player 1", TextureManager::GetTexture("Bro1Happy"), TextureManager::GetTexture("Bro1Sad"));
	Piece *orange2 = new Piece(ORANGE, "Player 2", TextureManager::GetTexture("Bro2Happy"), TextureManager::GetTexture("Bro2Sad"));

	if (player == "Player 1")
	{
		pieceGroup.Add(green1);
		pieceGroup.Add(blue1);
		pieceGroup.Add(pink1);
		pieceGroup.Add(orange1);

		pieces.push_back(green1);
		pieces.push_back(blue1);
		pieces.push_back(pink1);
		pieces.push_back(orange1);
	}
	else
	{
		pieceGroup.Add(green2);
		pieceGroup.Add(blue2);
		pieceGroup.Add(pink2);
		pieceGroup.Add(orange2);

		pieces.push_back(green2);
		pieces.push_back(blue2);
		pieces.push_back(pink2);
		pieces.push_back(orange2);
	}

	// board needs every piece to correctly show both player's moves
	board.addPiece(green1);
	board.addPiece(blue1);
	board.addPiece(pink1);
	board.addPiece(orange1);
	board.addPiece(green2);
	board.addPiece(blue2);
	board.addPiece(pink2);
	board.addPiece(orange2);

	pieceGroup.calculate();
	board.setHrzLayoutGrp(pieceGroup);
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
	sf::Texture t;
	t.loadFromFile("../assets/Background_1.png");
	sf::Sprite backgroundImage(t);

	// Create Board
	CardProps cardProps;
	VertexProps vertexProps;
	Coalition coalition;
	std::vector<Piece *> pieces;
	Board board;
	board.loadBoard("../files/board1.txt", vertexProps);

	// Create Button
	Button continueButton(&(TextureManager::GetTexture("continue")), &(TextureManager::GetTexture("continue_clicked")), sf::Vector2f(600.0f, 200.0f));
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

			// check if player's turn

			if (client.getIdentity() == client.getPlayerTurn())
			{

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
						v->setPiece(*p);
						v->setPlayer(client.getIdentity());

						client.sendNeighborColorPacket(board.updateBoard(v->getId()), socket);

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

		window.draw(backgroundImage);
		window.draw(board);

		window.display();
	}

	// Disconnect from the server
	socket.disconnect();
	receiveThread.join();

	return 0;
}
