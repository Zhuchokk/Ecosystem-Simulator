#ifndef MAP_H
#define MAP_H


#include <SFML/Graphics.hpp>
#include "../../Field.h"
#include "../../Commons.h"

class Map {
public:
    Map(uint16_t n, uint16_t m, float CELL_SIZE, const sf::Vector2f& thePosition,
        const sf::Vector2f& theSize, sf::PrimitiveType type, float _food_density, float water_percentage);

    const sf::View& get_view() const;
    const sf::VertexArray& get_vertices() const;
    void new_center_view(sf::Vector2f& currentCenter);
    void map_zoom(float factor);

private:
    sf::View mapView;
    uint16_t n, m;
    sf::VertexArray vertices;
    Field field;
    const float MARGIN = 30.0f;
    const float CELL_SIZE;

    void create_map();
};

#endif //MAP_H
