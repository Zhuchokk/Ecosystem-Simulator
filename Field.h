#pragma once
#include "Commons.h"

class Field {
private:
    int water_density;
    double food_density;
    Object*** matrix;
    uint16_t height;
    uint16_t width;
    
    void generation();
    int water_maker(uint16_t x, uint16_t y, uint16_t r);
public:
    Field(uint16_t _height, uint16_t _width, double _food_density, double water_percentage);
    ~Field();

    uint16_t get_width();
    uint16_t get_height();

    void del(uint16_t i, uint16_t j);
    Object* get(uint16_t i, uint16_t j);
    bool set(uint16_t i, uint16_t j, Object* obj);

    bool move_object_right(uint16_t i, uint16_t j);
    bool move_object_left(uint16_t i, uint16_t j);
    bool move_object_up(uint16_t i, uint16_t j);
    bool move_object_down(uint16_t i, uint16_t j);
};