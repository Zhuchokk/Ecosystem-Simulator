#include "../inc/Statistics.h"

Statistics::Statistics(const sf::Vector2f& thePosition, const sf::Vector2f& theSize) {
    rightView.setViewport(sf::FloatRect(thePosition, theSize));
    createRectangle(0.97f, 500, {0, 0});
}

const sf::View& Statistics::get_view() const {
    return rightView;
}

const sf::RectangleShape& Statistics::get_rectangle() const {
    return rightRect;
}

void Statistics::createRectangle(const float width, const float height, const sf::Vector2f& position) {
    rightRect.setSize({rightView.getSize().x * width, rightView.getSize().y + height});
    rightRect.setFillColor(bgColor);
    rightRect.setPosition(position);
}