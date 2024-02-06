
#include <SFML/Network.hpp>
#include <iostream>

int main() {
    // set server IP address and port number
    sf::IpAddress ip = "192.168.19.131";
    //sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    int port = 2222;

    // Connect to the server
    sf::TcpSocket socket;
    if (socket.connect(ip, 2222) != sf::Socket::Done) {
        std::cerr << "Error: Failed to connect to the server" << std::endl;
        return 1;
    }

    // Receive identity from the server
    sf::Packet identityPacket;
    if (socket.receive(identityPacket) != sf::Socket::Done) {
        std::cerr << "Error: Failed to receive identity from the server" << std::endl;
        return 1;
    }

    std::string identity;
    identityPacket >> identity;
    std::cout << "Server says: " << identity << std::endl;

    // Turn-based messaging loop
    while (true) {
        // If it's payer 1's turn, send message
        if (identity == "You are player 1") {
            std::string message;
            std::cout << "Enter your message: ";
            std::getline(std::cin, message);

            // Send message to the server
            sf::Packet packet;
            packet << message;
            if (socket.send(packet) != sf::Socket::Done) {
                std::cerr << "Error: Failed to send message to the server" << std::endl;
                break;
            }
        }

        // Receive message from the server
        sf::Packet receivedPacket;
        if (socket.receive(receivedPacket) != sf::Socket::Done) {
            std::cerr << "Error: Failed to receive message from the server" << std::endl;
            break;
        }

        std::string receivedMessage;
        receivedPacket >> receivedMessage;
        std::cout << "Server says: " << receivedMessage << std::endl;

        // If it's player 2's turn, send message
        if (identity == "You are player 2") {
            std::string message;
            std::cout << "Enter your message: ";
            std::getline(std::cin, message);

            // Send message to the server
            sf::Packet packet;
            packet << message;
            if (socket.send(packet) != sf::Socket::Done) {
                std::cerr << "Error: Failed to send message to the server" << std::endl;
                break;
            }
        }
    }

    // Disconnect from the server
    socket.disconnect();

    return 0;
}
