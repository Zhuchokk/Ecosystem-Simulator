#ifndef BASEVIEW_H
#define BASEVIEW_H

#include <SFML/Graphics.hpp>

class BaseView {
public:
    BaseView(const sf::Vector2f& thePosition, const sf::Vector2f& theSize);

    const sf::View& get_view() const;
    const sf::RectangleShape& get_rectangle() const;

private:
    sf::View leftView;
    sf::Color bgColor = {229, 229, 229};
    sf::RectangleShape leftRect;

    void createRectangle(float width, float height);
};

#endif //BASEVIEW_H
