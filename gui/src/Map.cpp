#include "../inc/Map.h"

Map::Map(const uint16_t n, const uint16_t m, const float CELL_SIZE, const sf::Vector2f& thePosition,
         const sf::Vector2f& theSize, const sf::PrimitiveType type, const float _food_density, const float water_percentage) :
    n(n), m(m), CELL_SIZE(CELL_SIZE), field(n, m, _food_density, water_percentage), vertices(type, n * m * 6) {
    mapView.setViewport(sf::FloatRect(thePosition, theSize));
    mapView.setCenter({n * 5.0f, m * 5.0f});

    create_map();
}

const sf::View& Map::get_view() const {
    return mapView;
}

const sf::VertexArray& Map::get_vertices() const {
    return vertices;
}

void Map::new_center_view(sf::Vector2f& currentCenter) {
    const float mapWidth = n * CELL_SIZE;
    const float mapHeight = m * CELL_SIZE;
    const sf::Vector2f viewSize = mapView.getSize();
    currentCenter.x = std::max(viewSize.x / 2.f + MARGIN, std::min(currentCenter.x, mapWidth - viewSize.x / 2.f - MARGIN));
    currentCenter.y = std::max(viewSize.y / 2.f + MARGIN, std::min(currentCenter.y, mapHeight - viewSize.y / 2.f - MARGIN));

    mapView.setCenter(currentCenter);
}

void Map::map_zoom(const float factor) {
    mapView.zoom(factor);
    const sf::Vector2f viewSize = mapView.getSize();
    mapView.setCenter({viewSize.x / 2.f + MARGIN, viewSize.y / 2.f + MARGIN});
}

void Map::create_map() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            float x = MARGIN + j * CELL_SIZE;
            float y = MARGIN + i * CELL_SIZE;

            int index = (i * m + j) * 6;

            vertices[index + 0].position = sf::Vector2f(x, y);
            vertices[index + 1].position = sf::Vector2f(x + CELL_SIZE, y);
            vertices[index + 2].position = sf::Vector2f(x + CELL_SIZE, y + CELL_SIZE);

            vertices[index + 3].position = sf::Vector2f(x, y);
            vertices[index + 4].position = sf::Vector2f(x + CELL_SIZE, y + CELL_SIZE);
            vertices[index + 5].position = sf::Vector2f(x, y + CELL_SIZE);

            Object* cell = field.get(i, j);
            sf::Color clr = (cell == nullptr) ? sf::Color::Black : sf::Color::White;

            for (int k = 0; k < 6; k++)
                vertices[index + k].color = clr;
        }
    }
}