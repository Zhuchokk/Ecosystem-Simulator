#ifndef STATISTICS_H
#define STATISTICS_H


#include <SFML/Graphics.hpp>

class Statistics {
public:
    Statistics(const sf::Vector2f& thePosition, const sf::Vector2f& theSize);

    const sf::View& get_view() const;
    const sf::RectangleShape& get_rectangle() const;

private:
    sf::View rightView;
    sf::Color bgColor = {219, 234, 254};
    sf::RectangleShape rightRect;

    void createRectangle(const float width, const float height, const sf::Vector2f& position);
};

#endif //STATISTICS_H
