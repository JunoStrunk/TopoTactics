
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
#include <vector>
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
	Card *selectedCard;
	Piece *selectedPiece;
	bool isCardSelected;
	bool isPieceSelected;

	int currentTurn;
	// Card IDs: 0 = reserved seating, 1 = embarassing family, 2 = rearrangement, 3 = inside joke
	// Array contains turns at which the effects of each card end. Begin at 0 (not in effect) and assigned a turn when the card is activated
	int targetTurn[4];
	// For use with selecting seats/factions after activating a card
	int activatingCard;
	Vertex *reservedSeat;
	std::vector<Vertex*> guestsToSwap;
	Coalition insideJoke;

public:
	int score;

	Client()
	{
		selectedPiece = nullptr;
		isPieceSelected = false;
		currentTurn = 1;
		score = 0;
		for (int i = 0; i < 4; i++) {
			targetTurn[i] = 0;
		}
		activatingCard = -1;
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

	bool hasSelectedCard() {
		return isCardSelected;
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

	int getActivatingCard() {
		return activatingCard;
	}

	void setActivatingCard(int card) {
		activatingCard = card;
	}

	Vertex* getReservedSeat() {
		return reservedSeat;
	}

	void setReservedSeat(Vertex *seat) {
		reservedSeat = seat;
	}

	void incrementTurn() {
		currentTurn++;

		if (currentTurn <= targetTurn[0] && reservedSeat != nullptr) { // end "reserved seating" card effect
			reservedSeat->setReserved(false);
			reservedSeat = nullptr;
		}
		if (currentTurn <= targetTurn[1]) { // end "embarrassing family" effect
			// TODO (based on how it is implemenented, see the TODO in main)
		}
		// no action required here for card id == 2 or 3
	}

	bool insideJokeInEffect() {
		return currentTurn <= targetTurn[3];
	}

	void setTargetTurn(int id, int turns) {
		targetTurn[id] = currentTurn + turns;
	}

	void addGuestToSwap(Vertex* vertex) {
		guestsToSwap.push_back(vertex);
	}

	bool canSwap() {
		return guestsToSwap.size() == 2;
	}

	Vertex* getGuest(int index) {
		return guestsToSwap[index];
	}

	void clearGuestsToSwap() {
		guestsToSwap.clear();
	}

	Coalition getInsideJoke() {
		return insideJoke;
	}

	void setInsideJoke(Coalition coalition) {
		insideJoke = coalition;
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

		for (auto i = graph.begin(); i != graph.end(); i++)
		{
			Vertex *v = i->second.first;

			if (v->getColor() != sf::Color::White && v->getOutlineColor() != sf::Color::Red)
			{
				if (v->getPlayer() == "Player 1") {
					client.score++;
					if (client.insideJokeInEffect() && v->getCoal() == client.getInsideJoke()) {
						client.score++;
					}
				}
			}
		}
		std::cout << "Current score: " << client.score << std::endl;
	}
}

void loadCards(Board &board, std::vector<Card *> &cards)
{
	Card* card0 = new Card(0, 100, 325);
	board.addCard(card0);
	cards.push_back(card0);
	Card* card1 = new Card(1, 250, 325);
	board.addCard(card1);
	cards.push_back(card1);
	Card* card2 = new Card(2, 400, 325);
	board.addCard(card2);
	cards.push_back(card2);
	Card* card3 = new Card(3, 550, 325);
	board.addCard(card3);
	cards.push_back(card3);
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
	std::vector<Card *> cards;
	std::vector<Piece *> pieces;
	Board board;
	board.loadBoard("../files/board1.txt", vertexProps);

	// Create Button
	Button continueButton(&(textureManager.GetTexture("UI/continue")), &(textureManager.GetTexture("UI/continue_clicked")), sf::Vector2f(600.0f, 200.0f));
	std::function<void()> continueButtonTest = std::bind(printMessage, "ContinueButtonWorks!");
	continueButton.bindOnClick(continueButtonTest);

	loadCards(board, cards);
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
				if (event.type == sf::Event::MouseButtonReleased) {
					if (client.getActivatingCard() == 0) { // "Reserved seating"--selecting an empty seat
						Vertex *v = board.mouseReleased(event);
						if (v != nullptr)
						{
							if (!v->getReserved() && !v->getHasPiece()) // if seat isn't already reserved or has piece, reserve it
							{
								v->setReserved(true);
								v->setPlayer(client.getIdentity());
								client.setReservedSeat(v);
								client.setTargetTurn(0, 2); // set reserved seating card effect to end two turns from now

								// TODO: update vertices with other client

								client.setActivatingCard(-1); // finished, stop card activation
							}
						}
					}
					else if (client.getActivatingCard() == 1) { // "Embarassing family"--select a faction to make unhappy
						Piece *p = board.mouseClickPiece(event, pieces);
						if (p != nullptr) {
							Coalition embarrassed = p->getCoal();
							// TODO: update board to make all pieces of the above coalition unhappy
							client.setTargetTurn(1, 2);
							client.setActivatingCard(-1);
						}
					}
					else if (client.getActivatingCard() == 2) { // "Rearrangement"--swap two of your guests
						Vertex *v = board.mouseReleased(event);
						if (v != nullptr) {
							if (v->getPlayer() == client.getIdentity()) { // add guest if it belongs to the player
								client.addGuestToSwap(v);
							}
							if (client.canSwap()) { // if  two guests have been clicked on, swap them
								Coalition v0 = client.getGuest(0)->getCoal();
								client.getGuest(0)->setCoal(client.getGuest(1)->getCoal());
								client.getGuest(1)->setCoal(v0);
								// TODO update board to reflect the swap
								client.clearGuestsToSwap();
								client.setActivatingCard(-1);
							}
						}
					}
					else if (client.getActivatingCard() == 3) { // "Inside joke"--select a faction, your guests of that faction will receive double happiness points
						Piece *p = board.mouseClickPiece(event, pieces);
						if (p != nullptr) {
							client.setInsideJoke(p->getCoal());
							client.setTargetTurn(3, 1); // set card effect for one turn
							client.setActivatingCard(-1);
						}
					}
					else if (!client.hasSelectedPiece()) {
						Card *c = board.mouseClickCard(event, cards);
						Piece *p = board.mouseClickPiece(event, pieces);

						if (c != nullptr) { // Card was clicked
							client.setActivatingCard(c->getID());
							c->setPlayed(true);
						}
						else if (p != nullptr)
						{

							client.setSelectedPiece(p);
							client.setHasSelectedPiece(true);
						}
					}
					else {
						Piece *p = client.getSelectedPiece();

						Vertex *v = board.mouseReleased(event);

						if (v != nullptr)
						{
							std::cout << "Reserved?" << v->getReserved() << std::endl;
							if (!v->getReserved() || (v->getReserved() && v->getPlayer() == client.getIdentity())) { // place piece only if seat isn't reserved by other player
								v->setIsHappy(true);
								v->setHasPiece(true);
								v->setPlayer(client.getIdentity());
								client.incrementTurn();

								std::vector<std::pair<Vertex *, sf::Color>> changedVetrices = client.setVertexColors(p, v, board);
								client.sendNeighborColorPacket(changedVetrices, socket);

								client.setHasSelectedPiece(false);
								// client.calculateScore(board);
								break;
							}
						}
					}
				}
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
