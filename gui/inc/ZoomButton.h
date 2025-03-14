#ifndef ZOOMBUTTON_H
#define ZOOMBUTTON_H


#include "Button.h"

class ZoomButton : public Button {
public:
    ZoomButton(float x, float y, const sf::Vector2f& size, const sf::Color& color, const sf::Font& font,
               const std::string& text, unsigned int fontSize, const sf::Color& textColor, float zoomFactor);

    float getZoomFactor() const;

private:
    float zoomFactor;
};

#endif //ZOOMBUTTON_H
