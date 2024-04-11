#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "Card.h"
#include "Button.h"
#include "TextureManager.h"
#include "CardProps.h"
#include "VertexProps.h"
#include "Board.h"

static Board board;

void printMessage(const std::string &msg)
{
	std::cout << msg << std::endl;
}

void imGuiWindow()
{
	ImGui::Begin("Edit Board");
	ImGui::Checkbox("Editable", board.getEditing());
	ImGui::End();
}

int main()
{
	// Window Properties
	const sf::Color WINDOW_COLOR = sf::Color::White;
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Test window");
	if (!(ImGui::SFML::Init(window)))
		return -1;
	sf::Clock deltaClock;
	TextureManager texManager;

	// Create Board
	CardProps cardProps;
	VertexProps vertexProps;
	board.loadBoard("../files/board1.txt", vertexProps);

	// Create Button
	Button continueButton(&(texManager.GetTexture("UI/continue")), &(texManager.GetTexture("UI/continue_clicked")), sf::Vector2f(600.0f, 200.0f));
	std::string continueMessage = "ContinueButtonWorks!";
	std::function<void()> continueButtonTest = [&continueMessage]()
	{ printMessage(continueMessage); };

	continueButton.bindOnClick(continueButtonTest);

	// Start the game loop
	while (window.isOpen())
	{
		// Clear screen
		window.clear(WINDOW_COLOR);

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonReleased)
			{
				board.mouseReleased(event);
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				board.mousePressed(event);
			}
			if (event.type == sf::Event::MouseMoved)
			{
				board.mouseMoved(event);
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		imGuiWindow();

		window.draw(board);
		window.draw(continueButton);
		ImGui::SFML::Render(window);

		window.display();
	}
	ImGui::SFML::Shutdown();

	return EXIT_SUCCESS;
}