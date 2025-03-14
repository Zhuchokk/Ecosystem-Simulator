#pragma once

#include <iostream>
#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "inc/Button.h"
#include "inc/ZoomButton.h"
#include "inc/Map.h"
#include "inc/Statistics.h"
#include "inc/BaseView.h"



int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Ecosystem-Simulator", sf::Style::Default, sf::State::Windowed);
    window.setFramerateLimit(30);// call it once after creating the window

    sf::Color backgroundColor(240, 240, 240);

    // mapView
    Map gameMap(300, 300, 10.0f, {0.01, 0.01},
        {0.68f, 0.98f}, sf::PrimitiveType::Triangles, 0.35, 0.6);

    Statistics rightView({0.7f, 0}, {0.3f, 1.0f}); // Правый сектор занимает 30% шир
    BaseView leftView({0, 0}, {0.7f, 1.0f}); // Левый сектор (70%)


    // Шрифт и текст
    sf::Font font;
    if (!font.openFromFile("../resources/fonts/arialmt.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    ZoomButton zoomInButton(50.f, 50.f, {50.f, 50.f}, sf::Color::Green, font, "+", 30, sf::Color::Black, 0.8f);
    ZoomButton zoomOutButton(110.f, 50.f, {50.f, 50.f}, sf::Color::Yellow, font, "-", 30, sf::Color::Black, 1.2f);


    float scrollOffset = 0.0f;
    bool mouse_pressed = false;

    sf::Vector2i lastMousePosition;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }

            if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if(mouseWheelScrolled->wheel == sf::Mouse::Wheel::Vertical)
                    if(mouseWheelScrolled->position.x >= window.getSize().x * 0.7)
                        scrollOffset -= mouseWheelScrolled->delta * 20.0f;
            }

            if(const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {

                if(mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    lastMousePosition = mouseButtonPressed->position;
                    mouse_pressed = true;
                }

                sf::Vector2f mousePos(static_cast<float>(mouseButtonPressed->position.x), static_cast<float>(mouseButtonPressed->position.y));
                if (zoomInButton.isClicked(mousePos)) {
                    gameMap.map_zoom(0.8f); // up
                    window.setView(gameMap.get_view());
                }
                if (zoomOutButton.isClicked(mousePos)) {
                    gameMap.map_zoom(1.2f); // down
                    window.setView(gameMap.get_view());
                }

            }

            if(const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                if(mouse_pressed) {
                    sf::Vector2i currentMousePosition = mouseMoved->position;
                    sf::Vector2i mouseDelta = currentMousePosition - lastMousePosition;

                    // Обновляем центр вида
                    sf::Vector2f currentCenter = gameMap.get_view().getCenter();
                    currentCenter.x -= mouseDelta.x; // Инверсия для естественного движения
                    currentCenter.y -= mouseDelta.y;

                    gameMap.new_center_view(currentCenter);

                    // Ограничиваем перемещение
                    lastMousePosition = currentMousePosition;
                }
            }

            if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                    mouse_pressed = false;
        }

        // clear the window with black color
        window.clear(backgroundColor);


        window.setView(leftView.get_view());
        window.draw(leftView.get_rectangle());

        window.setView(gameMap.get_view());
        window.draw(gameMap.get_vertices());


        window.setView(rightView.get_view());
        window.draw(rightView.get_rectangle());

        window.setView(window.getDefaultView());
        zoomInButton.draw(window);
        zoomOutButton.draw(window);

        window.display();
    }
}