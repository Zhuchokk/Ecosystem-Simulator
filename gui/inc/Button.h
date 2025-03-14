#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button(float x, float y, const sf::Vector2f& size, const sf::Color& color, const sf::Font& font,
           const std::string& text, unsigned int fontSize, const sf::Color& textColor);

    void draw(sf::RenderWindow& window) const;
    bool isClicked(const sf::Vector2f& mousePos) const;

protected:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
};

#endif //BUTTON_H
