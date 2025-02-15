#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Ecosystem-Simulator", sf::Style::Default, sf::State::Windowed);
    window.setVerticalSyncEnabled(true); // call it once after creating the window

    sf::Color backgroundColor(197, 52, 219);


    // double zone
    sf::View leftView;  // Левый сектор (70%)
    sf::View rightView; // Правый сектор (30%)

    // Настройка видов
    leftView.setViewport(sf::FloatRect({0, 0}, {0.7f, 1.0f}));  // Левый сектор занимает 70% ширины
    rightView.setViewport(sf::FloatRect({0.7f, 0}, {0.3f, 1.0f})); // Правый сектор занимает 30% шир


    // font

    sf::Font font;
    if (!font.openFromFile("./resources/fonts/arialmt.ttf"))
    {
        std::cout << "where is font?" << std::endl;
    }
    sf::Text leftText(font);
    leftText.setString("Left Section (70%)");
    leftText.setCharacterSize(24);
    leftText.setFillColor(sf::Color::White);
    leftText.setPosition({50, 50});

    sf::Text rightText(font);
    rightText.setString("Right Section (30%)");
    rightText.setCharacterSize(24);
    rightText.setFillColor(sf::Color::White);
    rightText.setPosition({600, 50});


    sf::Texture bush;
    if (!bush.loadFromFile("./resources/textures/plants.png", false, sf::IntRect({120, 40}, {100, 96})))
    {
        std::cout << "where is texture?" << std::endl;
    }
    bush.setSmooth(true);// Throws sf::Exception if an error occurs

    sf::Sprite sprite(bush);

    sf::RectangleShape leftRect(sf::Vector2f(200, 200));
    leftRect.setFillColor(sf::Color::Red);
    leftRect.setPosition({100, 100});

    // Создаем длинный прямоугольник для правого сектора (чтобы было что прокручивать)
    sf::RectangleShape rightRect(sf::Vector2f(150, 1200)); // Высота 1200 пикселей
    rightRect.setFillColor(sf::Color::Blue);
    rightRect.setPosition({600, 0}); // Правый прямоугольник начинается в верхней части



    float scrollOffset = 0.0f;
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

            if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if(mouseWheelScrolled->wheel == sf::Mouse::Wheel::Vertical) {
                    scrollOffset -= mouseWheelScrolled->delta * 10.0f;
                }
            }

        }

        // Ограничиваем прокрутку, чтобы не выйти за пределы содержимого
        scrollOffset = std::max(scrollOffset, 0.0f); // Не прокручиваем выше начала
        scrollOffset = std::min(scrollOffset, rightRect.getSize().y - 1000.0f); // Не прокручиваем ниже конца

        rightView.setCenter({700, 300 + scrollOffset}); // Центр вида смещается в зависимости от прокрутки

        // clear the window with black color
        window.clear(backgroundColor);

        window.setView(leftView);
        window.draw(leftText);

        window.setView(rightView);
        window.draw(rightText);

        window.draw(sprite);
        // end the current frame
        window.display();
    }
}