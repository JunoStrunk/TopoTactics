
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread> 

// set game constants
// client
const sf::IpAddress IP = "192.168.19.131";
//const sf::IpAddress IP = sf::IpAddress::getLocalAddress();
const int PORT = 2222;
// window
const sf::Color WINDOW_COLOR = sf::Color::White;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 500;
// cards
const int CARD_WIDTH = 60;
const int CARD_HEIGHT = 90;
const int NUM_CARDS = 5;
const int CARD_OUTLINE_THICKNESS = 5;
const sf::Color CARD_FILL_COLOR = sf::Color::Transparent;
const sf::Color CARD_OUTLINE_COLOR = sf::Color::Black;
const sf::Color CARD_SELECTED_OUTLINE_COLOR = sf::Color::Yellow;
// nodes
const int NUM_NODES = 3;
const float NODE_SIZE = 30;
const int NODE_OUTLINE_THICKNESS = 3;
const sf::Color NODE_FILL_COLOR = sf::Color::Green;
const sf::Color NODE_OUTLINE_COLOR = sf::Color::Black;
const sf::Color NODE_SELECTED_OUTLINE_COLOR = sf::Color::Magenta;

class Edge;

class Node {
    sf::CircleShape shape;
    bool hovered;
    bool selected;
    std::vector<Edge*> edges;
    float xpos;
    float ypos;

public:
    // Node constructor
    Node(float x, float y) {
        xpos = x;
        ypos = y;
        shape.setPosition(xpos, ypos);
        shape.setRadius(NODE_SIZE);
        shape.setFillColor(NODE_FILL_COLOR);
        shape.setOrigin(NODE_SIZE, NODE_SIZE);
        shape.setOutlineThickness(NODE_OUTLINE_THICKNESS);
        shape.setOutlineColor(NODE_OUTLINE_COLOR);
        hovered = false;
        selected = false;
    }
    float getX() {
        return xpos;
    }

    float getY() {
        return ypos;
    }

    void addEdge(Edge* edge) {
        edges.push_back(edge);
    }

    std::vector<Edge*> getEdges() {
        return edges;
    }

    sf::CircleShape getShape() {
        return shape;
    }
    bool isHovered() {
        return hovered;
    }
    void setHovered(bool hover) {
        hovered = hover;
    }
    void setSelected(bool select) {
        if (select) {
            selected = true;
            //shape.setOutlineColor(NODE_OUTLINE_COLOR);
        }
        else {
            selected = false;
            
        }
    }

    bool isSelected() {
        return selected;
    }

    void setColor(sf::Color color) {
        shape.setFillColor(color);
    }

    sf::Color getColor() {
        return shape.getFillColor();
    }

};

class Edge {
    Node* endpoint;
public:
    Edge(Node* endpointNode) {

        endpoint = endpointNode;
    }
    Node* getEndpoint() {
        return endpoint;
    }
};

class Card {
    sf::RectangleShape shape;
    bool hovered;
    bool selected;

public:
    // card constructor
    Card(float x, float y) {

        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(CARD_WIDTH, CARD_HEIGHT));
        shape.setFillColor(CARD_FILL_COLOR);
        shape.setOutlineThickness(CARD_OUTLINE_THICKNESS);
        shape.setOutlineColor(CARD_OUTLINE_COLOR);
        hovered = false;
        selected = false;
    }

    sf::RectangleShape getShape() {
        return shape;
    }
    bool isHovered() {
        return hovered;
    }
    void setHovered(bool hover) {
        hovered = hover;
    } 
    void setSelected(bool select) {
        if (select) {
            shape.setOutlineColor(CARD_SELECTED_OUTLINE_COLOR);
            selected = true;
            //hovered = true;
        }
        else {
            shape.setOutlineColor(CARD_OUTLINE_COLOR);
            selected = false;
            //hovered = false;
        }
    }

    bool isSelected() {
        return selected;
    }

};

class Piece {

public:
    Piece(){}
};

class Client {
    std::string identity;
    std::string playerTurn;


public:
    Client() {}

    void initailize(sf::TcpSocket& socket) {

        // set server IP address and port number
        sf::IpAddress ip = IP;
        int port = PORT;

        // Connect to the server
        if (socket.connect(ip, port) != sf::Socket::Done) {
            std::cerr << "Error Failed to connect to the server" << std::endl;
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

    void sendCircleColorPacket(std::vector<Node*>& nodes, sf::TcpSocket& socket) {

        //create packet to send
        sf::Packet packet;

        // Create a message packet and send it to the server
        // We can use the Packet class to build blocks of data to transfer over the network
        // we can send a node position, for example position 10
        // and a coalition, for example green
        // packet << 10 << "green"; 

        //change circle color
        //send mesage to server to request element change and wait for a response

        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i]->isSelected()) {
                // build packet with node number and color

                if (nodes[i]->getColor() == sf::Color::Blue) {
                    packet << i << "green";
                }
                else {
                    packet << i << "blue";
                }
            }
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

    void initializeWindow(sf::RenderWindow& window, std::vector<Node*>& nodes, std::vector<Edge*>& edges, std::vector<Card*>& cards) {
    
        // create cards
        for (int i = 0; i < NUM_CARDS; i++) {
            float x = i * (CARD_WIDTH + 20) + 10;
            float y = WINDOW_HEIGHT - CARD_HEIGHT - 10;
            
            Card* card = new Card(x, y);
            cards.push_back(card);
        }

        // create nodes
        for (int i = 0; i < NUM_NODES; i++) {
            float x = i * (NODE_SIZE + 100) + 100;
            float y = NODE_SIZE + 10;

            Node* node = new Node(x, y);
            nodes.push_back(node);
        }

        // create edges
        for (int i = 0; i < NUM_NODES; i++) {
            if (i + 1 < NUM_NODES) {
                Edge* edge = new Edge(nodes[i + 1]);
                edges.push_back(edge);

                nodes[i]->addEdge(edge);
            }
        }
    }

    void drawNodes(sf::RenderWindow& window, std::vector<Node*>& nodes) {
        for (int i = 0; i < NUM_NODES; i++) {
            sf::CircleShape nodeShape = nodes[i]->getShape();
            if (nodes[i]->isHovered()) {
                //nodeShape.setRadius(NODE_SIZE + 10);
                nodeShape.setOutlineColor(NODE_SELECTED_OUTLINE_COLOR);
            }
            else {
                nodeShape.setOutlineColor(NODE_OUTLINE_COLOR);
            }
            window.draw(nodeShape);
        }
    }

    void drawEdges(sf::RenderWindow& window, std::vector<Node*>& nodes) {

        std::vector<sf::VertexArray> lines;

        for (int i = 0; i < nodes.size(); i++) {
            std::vector<Edge*> edges = nodes[i]->getEdges();
            for (int j = 0; j < edges.size(); j++) {
                sf::VertexArray line(sf::Lines, 2);
                line[0].position = sf::Vector2f(nodes[i]->getX(), nodes[i]->getY());
                line[1].position = sf::Vector2f(edges[j]->getEndpoint()->getX(), edges[j]->getEndpoint()->getY());
                line[0].color = sf::Color::Black;
                line[1].color = sf::Color::Black;
                lines.push_back(line);
            }
        }

        for (int i = 0; i < lines.size(); i++) {
            window.draw(lines[i]);
        }
    }

    void drawCards(sf::RenderWindow& window, std::vector<Card*>& cards) {
        for (int i = 0; i < NUM_CARDS; i++) {
            sf::RectangleShape cardShape = cards[i]->getShape();
            if (cards[i]->isHovered()) {
                cardShape.move(0, -20);
            }
            window.draw(cardShape);
        }
    }

    // clear game elements, call before initialize when starting a new game
    void clear(sf::RenderWindow& window, std::vector<Node*>& nodes, std::vector<Edge*>& edges, 
               std::vector<Card*>& cards, std::vector<Piece*>& pieces) {
        // delete nodes
        for (int i = 0; i < nodes.size(); i++) {
            delete nodes[i];
        }
        nodes.clear();

        // delete edges
        for (int i = 0; i < edges.size(); i++) {
            delete edges[i];
        }
        edges.clear();

        // delete cards
        for (int i = 0; i < cards.size(); i++) {
            delete cards[i];
        }
        cards.clear();

        // delete pieces
        for (int i = 0; i < pieces.size(); i++) {
            delete pieces[i];
        }
        pieces.clear();
    }
    


};

class Input {
    bool boardSelected;

public:

    Input() {
        boardSelected = false;
    }

    bool isBoardSelected() {
        return boardSelected;
    }

    void selectBoard() {
        boardSelected = true;
    }

    void mouseHoverOnNode(sf::Event& event, std::vector<Node*>& nodes, sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i]->getShape().getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                nodes[i]->setHovered(true);
            }
            else {
                nodes[i]->setHovered(false);
            }
        }
    }
    
    bool mouseClickOnNode(sf::Event& event, std::vector<Node*>& nodes, sf::RenderWindow& window) {

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        for (int i = 0; i < nodes.size(); i++) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                //check if mouse click is in shape
                if (nodes[i]->getShape().getGlobalBounds().contains(mousePosition)) {
                    nodes[i]->setSelected(true);
                    return true;
                }

                else {
                    //return false;
                }
            }
        }
        return false;



        /*
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            //check if mouse click is in shape
            if (shape.getGlobalBounds().contains(mousePosition))
                return true;
            else
                return false;
        }
        return false;
        */
    }

    void mouseHoverOnCard(sf::Event& event, std::vector<Card*>& cards, sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        for (int i = 0; i < cards.size(); i++) {
            if (cards[i]->getShape().getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                cards[i]->setHovered(true);
            }
            else {
                cards[i]->setHovered(false);
            }
        }
    }

    bool mouseClickOnCard(sf::Event& event, std::vector<Card*>& cards, sf::RenderWindow& window) {
        
        int selectedCard = cardSelected(cards);

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        for (int i = 0; i < cards.size(); i++) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                //check if mouse click is in shape
                if (cards[i]->getShape().getGlobalBounds().contains(mousePosition)) {
                    if (selectedCard >= 0) {
                        cards[selectedCard]->setSelected(false);
                    }
                    
                    cards[i]->setSelected(true);
                    return true;
                }
                    
                else {
                    //return false;
                }
            }
        }
        return false;
    }
    int cardSelected(std::vector<Card*>& cards) {
        for (int i = 0; i < cards.size(); i++) {
            if (cards[i]->isSelected()) {
                return i;
            }
        }
        return -1;
    }
};



void receiveMessages(sf::TcpSocket& socket, std::vector<Node*>& nodes, Client& client) {
    while (true) {
        sf::Packet receivedPacket;
        if (socket.receive(receivedPacket) == sf::Socket::Done) {
            
            int nodeNum;
            std::string color;
            std::string playerTurn;
            receivedPacket >> nodeNum >> color >> playerTurn;

            std::cout << "player turn: " << playerTurn << std::endl;
            client.setPlayerTurn(playerTurn);

            sf::Color fillColor;

            if (color == "blue") {
                
                fillColor = sf::Color::Blue;
                //shape.setFillColor(sf::Color::Blue);
            }
            else {
                fillColor = sf::Color::Green;
                //shape.setFillColor(sf::Color::Green);
            }

            // set new fill color 
            nodes[nodeNum]->setColor(fillColor);

            // deselect node
            nodes[nodeNum]->setSelected(false);
            nodes[nodeNum]->setHovered(false);

        }
    }
}

int main() {

    // game elements
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    std::vector<Card*> cards;
    std::vector<Piece*> pieces;

    //create client object and initialize with socket
    Client client;
    sf::TcpSocket socket;
    client.initailize(socket);

    //create rendering object
    Rendering rendering;

    //setup window and initialize game elements
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TopoTactics!");
    rendering.initializeWindow(window, nodes, edges, cards);

    //create Input object
    Input input;

    //Start the message receiving thread
    std::thread receiveThread(receiveMessages, std::ref(socket), std::ref(nodes), std::ref(client));

    //Game loop
    while (window.isOpen()) {
        window.clear(sf::Color::White);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // select game board 
            while (!input.isBoardSelected()) {
                // TODO: pass .txt file to input, then render board from file??? 
                // rendering.clear() will delete all game elements if needed before starting a new game
                input.selectBoard();
            }

            // check if player's turn 
            if (client.getIdentity() == client.getPlayerTurn()) {
                
                // check if mouse on card
                input.mouseHoverOnCard(event, cards, window);

                // check if mouse click on card
                if (input.mouseClickOnCard(event, cards, window)) {
                    // set card hover 

                    // give play card or select new card option
                }

                // check if mouse on node
                input.mouseHoverOnNode(event, nodes, window);

                //check if mouse click is on a node
                if (input.mouseClickOnNode(event, nodes, window)) {
                    client.sendCircleColorPacket(nodes, socket);
                }
            }  
        }
 
        window.clear(WINDOW_COLOR);
        //window.draw(shape);
        rendering.drawNodes(window, nodes);
        rendering.drawEdges(window, nodes);
        rendering.drawCards(window, cards);

        window.display();
    }

    //Disconnect from the server
    receiveThread.join();
    socket.disconnect();

    return 0;
}