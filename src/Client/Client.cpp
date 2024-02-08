
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

class Client {
    std::string identity;
    std::string playerTurn;


public:
    Client() {}

    void initailize(sf::TcpSocket& socket) {

        // set server IP address and port number
        //sf::IpAddress ip = "127.0.0.1";
        sf::IpAddress ip = sf::IpAddress::getLocalAddress();
        int port = 2222;

        // Connect to the server

        if (socket.connect(ip, 2222) != sf::Socket::Done) {
            std::cerr << "Error Failed to connect to the server" <<  std::endl;
            // handle error
        }


        // Receive identity from the server
        sf::Packet identityPacket;
        if (socket.receive(identityPacket) != sf::Socket::Done) {
            std::cerr << "Error: Failed to receive identity from the server" << std::endl;
            //handle error
        }

        identityPacket >> identity >> playerTurn;
        std::cout << "You are " << identity << std::endl;
        std::cout << "player turn: " << playerTurn << std::endl;

    }

    std::string getIdentity() {
        return identity;
    }

    std::string getPlayerTurn() {
        return playerTurn;
    }

    void setPlayerTurn(std::string turn) {
        playerTurn = turn;
    }

    void sendCircleColorPacket(sf::CircleShape& shape, sf::TcpSocket& socket) {

        //create packet to send
        sf::Packet packet;

        // Create a message packet and send it to the server
        // We can use the Packet class to build blocks of data to transfer over the network
        // we can send a node position, for example position 10
        // and a coalition, for example green
        // packet << 10 << "green"; 

        //change circle color
        //send mesage to server to request element change and wait for a response
        if (shape.getFillColor() == sf::Color::Blue) {
            packet << "green";
        }

        else {
            packet << "blue";
        }

        // send packet to server
        if (socket.send(packet) != sf::Socket::Done) {
            std::cerr << "Error: Failed to send packet to the server" << std::endl;
            //handle error accordingly
        }
    }
};

class Rendering {

public:

    Rendering() {}

    void initializeWindow(sf::RenderWindow& window) {}
};

class Input {

public:

    Input() {}

    bool mouseClickOnShape(sf::Event& event, sf::CircleShape& shape, sf::RenderWindow& window) {

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            //check if mouse click is in shape
            if (shape.getGlobalBounds().contains(mousePosition))
                return true;
            else
                return false;
        }
        return false;
    }
};

void receiveMessages(sf::TcpSocket& socket, sf::CircleShape& shape, Client& client) {
    while (true) {
        sf::Packet receivedPacket;
        if (socket.receive(receivedPacket) == sf::Socket::Done) {
            std::string color;
            std::string playerTurn;
            receivedPacket >> color >> playerTurn;

            std::cout << "player turn: " << playerTurn << std::endl;
            client.setPlayerTurn(playerTurn);

            if (color == "blue") {
                shape.setFillColor(sf::Color::Blue);
            }
            else {
                shape.setFillColor(sf::Color::Green);
            }
        }
    }
}

int main() {

    //create client object and initialize with socket
    Client client;
    sf::TcpSocket socket;
    client.initailize(socket);

    //create rendering object
    Rendering rendering;

    //setup window and initial shapes
    sf::RenderWindow window(sf::VideoMode(200, 200), "TopoTactics!");
    //rendering.initializeWindow(window);

    //setup game elements
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    //create Input object
    Input input;

    //Start the message receiving thread
    std::thread receiveThread(receiveMessages, std::ref(socket), std::ref(shape), std::ref(client));

    //Game loop
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // check if player's turn 
            if (client.getIdentity() == client.getPlayerTurn()) {
                //check if mouse click is on a shape
                if (input.mouseClickOnShape(event, shape, window)) {
                    client.sendCircleColorPacket(shape, socket);
                }
            }  
        }
 
        window.clear();
        window.draw(shape);
        window.display();
    }

    //Disconnect from the server
    receiveThread.join();
    socket.disconnect();

    return 0;
}