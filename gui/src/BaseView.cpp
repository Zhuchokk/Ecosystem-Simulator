#include "../inc/BaseView.h"

BaseView::BaseView(const sf::Vector2f& thePosition, const sf::Vector2f& theSize) {
    leftView.setViewport(sf::FloatRect(thePosition, theSize));
    createRectangle(0.97f, 0.97f, {0, 0});
}

const sf::View& BaseView::get_view() const {
    return leftView;
}

const sf::RectangleShape& BaseView::get_rectangle() const {
    return leftRect;
}

void BaseView::createRectangle(const float width, const float height, const sf::Vector2f& position) {
    leftRect.setSize({leftView.getSize().x * width, leftView.getSize().y * height});
    leftRect.setFillColor(bgColor);
    leftRect.setPosition({leftView.getSize().x * 0.01f, leftView.getSize().y * 0.01f});
    leftRect.setOutlineColor(sf::Color::Black);
    leftRect.setOutlineThickness(2);
}