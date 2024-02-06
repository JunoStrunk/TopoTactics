
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

void receiveMessages(sf::TcpSocket& socket, sf::CircleShape& shape) {
    while (true) {
        sf::Packet receivedPacket;
        if (socket.receive(receivedPacket) == sf::Socket::Done) {
            std::string s;
            receivedPacket >> s;
            if (s == "blue") {
                shape.setFillColor(sf::Color::Blue);
            }
            else {
                shape.setFillColor(sf::Color::Green);
            }
        }
    }
}


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

    // setup window and initial shape color
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    // Start the message receiving thread
    std::thread receiveThread(receiveMessages, std::ref(socket), std::ref(shape));


    // Game loop
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // if player 1's turn, handle mouse click
            if (identity == "You are player 1") {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                        // check if mouse click is in shape
                        if (shape.getGlobalBounds().contains(mousePosition)) {

                            // create packet to send 
                            sf::Packet packet;

                            // change circle color
                            // ??? send mesage to server to request element change and wait for a response???
                            if (shape.getFillColor() == sf::Color::Blue) {
                                //shape.setFillColor(sf::Color::Green); // dont set fill color here

                                packet << "green";
                            }

                            else {
                                //shape.setFillColor(sf::Color::Blue); // dont set fill color here
                                packet << "blue";
                            }

                            // send packet to server
                            if (socket.send(packet) != sf::Socket::Done) {
                                std::cerr << "Error: Failed to send packet to the server" << std::endl;
                                // handle error accordingly
                                break;
                            }

                        }
                    }
                }
            }


            // If it's player 2's turn, handle mouse click
            if (identity == "You are player 2") {

                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                        if (shape.getGlobalBounds().contains(mousePosition)) {

                            /* 
                            * create packet to send to the server
                            * We will use the Packet class to build blocks of data to transfer over the network 
                            * we can send a node position, for example position 10
                            * and a coalition, for example green
                            * sf::Uint32 x = 10; 
					        * string s = "green";
                            * packet << x << s;
                            */
                            sf::Packet packet;
                            // change circle color
                            if (shape.getFillColor() == sf::Color::Blue) {

                                packet << "green";
                            }

                            else {
                                packet << "blue";
                            }

                            // send packet to server
                            if (socket.send(packet) != sf::Socket::Done) {
                                std::cerr << "Error: Failed to send packet to the server" << std::endl;
                                // handle error accordingly
                                break;
                            }


                        }

                    }


                }


            }

        }

        window.clear();
        // Draw game elements
        window.draw(shape);
        window.display();


    }

    // Disconnect from the server
    receiveThread.join();
    socket.disconnect();

    return 0;
}
