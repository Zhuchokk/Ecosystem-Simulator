#include "../inc/ZoomButton.h"

ZoomButton::ZoomButton(float x, float y, const sf::Vector2f& size, const sf::Color& color, const sf::Font& font,
                       const std::string& text, unsigned int fontSize, const sf::Color& textColor, float zoomFactor) :
    Button(x, y, size, color, font, text, fontSize, textColor), zoomFactor(zoomFactor) {}

float ZoomButton::getZoomFactor() const {
    return zoomFactor;
}