#include <SFML/Network.hpp>
#include <iostream>

int main() {
    // choose a server port
    int port = 2222;
    // Create a TCP socket for the server
    sf::TcpListener listener;

    // Bind the listener to a port
    if (listener.listen(port) != sf::Socket::Done) {
        std::cerr << "Error: Failed to bind the listener to port " << port << std::endl;
        return 1;
    }

    std::cout << "Server is running, waiting for players to connect..." << std::endl;

    // Wait for client 1 to connect
    sf::TcpSocket player1;
    if (listener.accept(player1) != sf::Socket::Done) {
        std::cerr << "Error: Failed to accept player 1's connection" << std::endl;
        return 1;
    }

    std::cout << "Player 1 connected, waiting for player 2..." << std::endl;

    // Wait for player 2 to connect
    sf::TcpSocket player2;
    if (listener.accept(player2) != sf::Socket::Done) {
        std::cerr << "Error: Failed to accept player 2's connection" << std::endl;
        return 1;
    }

    std::cout << "Player 2 connected" << std::endl;

    // Assign player identities and player turn packets
    std::string playerIdentity = "Player1";
    std::string playerTurn = "Player1";

    sf::Packet identityPacket;
    identityPacket << playerIdentity << playerTurn;


    if (player1.send(identityPacket) != sf::Socket::Done) {
        std::cerr << "Error: Failed to send identity to player 1" << std::endl;
        return 1;
    }

    playerIdentity = "Player2";
    identityPacket.clear();
    identityPacket << playerIdentity << playerTurn;

    if (player2.send(identityPacket) != sf::Socket::Done) {
        std::cerr << "Error: Failed to send identity to player 2" << std::endl;
        return 1;
    }

    // Loop for turn-based messaging
    while (true) {
        // Receive message from player 1
        sf::Packet packet1;
        sf::Socket::Status player1Status = player1.receive(packet1);
        if (player1Status != sf::Socket::Done) {
            std::cerr << "Error: Failed to receive message from player 1" << std::endl;
            break;
        }
        else if (player1Status == sf::Socket::Done) {
       
            std::string s;
            packet1 >> s;
            packet1.clear();

            std::cout << "Received message from player 1: " << s << std::endl;

            playerTurn = "Player2";
            packet1 << s << playerTurn;
            
        }

        // 
        // Packet received form client 1, send packet back to player 1 and player 2 to set the fill color
        // a)
        // Forward message to player 2
        if (player2.send(packet1) != sf::Socket::Done) {
            std::cerr << "Error: Failed to send message to player 2" << std::endl;
            break;
        }

        // b)
        // also send packet back to player 1

        if (player1.send(packet1) != sf::Socket::Done) {
            std::cerr << "Error: Failed to send message to player 2" << std::endl;
            break;
        }
        

        // Receive message from player 2
        sf::Packet packet2;
        sf::Socket::Status player2Status = player2.receive(packet2);
        if (player2Status != sf::Socket::Done) {
            std::cerr << "Error: Failed to receive message from player 2" << std::endl;
            break;
        }
        else if (player2Status == sf::Socket::Done) {
            
            std::string s;
            packet2 >> s;
            packet2.clear();

            std::cout << "Received message from player 2: " << s << std::endl;

            playerTurn = "Player1";
            packet2 << s << playerTurn;
        }

        // 
        // Packet received from client 2, send packet back to player 1 and player 2 to set the fill color
        // 
        // a)
        // Forward message to player 1
        if (player1.send(packet2) != sf::Socket::Done) {
            std::cerr << "Error: Failed to send message to player 1" << std::endl;
            break;
        }

        // b)
        // also send packet back to player 2
        if (player2.send(packet2) != sf::Socket::Done) {
            std::cerr << "Error: Failed to send message to player 1" << std::endl;
            break;
        }
        
    }

    // Close connections
    player1.disconnect();
    player2.disconnect();
    listener.close();

    return 0;
}