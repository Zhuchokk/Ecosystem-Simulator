#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Ecosystem-Simulator", sf::Style::Default, sf::State::Windowed);

    window.setVerticalSyncEnabled(true); // call it once after creating the window

    sf::Texture bush;
    if (!bush.loadFromFile("./resources/textures/plants.png", false, sf::IntRect({120, 40}, {100, 96})))
    {
        std::cout << "where is texture?" << std::endl;
    }
    bush.setSmooth(true);// Throws sf::Exception if an error occurs

    sf::Sprite sprite(bush);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
        }

        // clear the window with black color
        window.clear(sf::Color::White);

        // draw everything here...
        // window.draw(...);

        window.draw(sprite);
        // end the current frame
        window.display();
    }
}