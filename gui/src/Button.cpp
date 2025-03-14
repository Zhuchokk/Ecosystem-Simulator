#include "../inc/Button.h"

Button::Button(float x, float y, const sf::Vector2f& size, const sf::Color& color, const sf::Font& font,
               const std::string& text, unsigned int fontSize, const sf::Color& textColor) : buttonText(font) {
    buttonShape.setSize(size);
    buttonShape.setFillColor(color);
    buttonShape.setPosition({x, y});

    buttonText.setString(text);
    buttonText.setCharacterSize(fontSize);
    buttonText.setFillColor(textColor);
    buttonText.setPosition({x + size.x / 3, y + size.y / 4});
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(buttonShape);
    window.draw(buttonText);
}

bool Button::isClicked(const sf::Vector2f& mousePos) const {
    return buttonShape.getGlobalBounds().contains(mousePos);
}