#pragma once
#include "Creatures.h"

class Field {
private:
    int water_density;
    double food_density;
    Object*** matrix;
    int height;
    int width;
    
    void generation();
    int water_maker(int x, int y, int r);
public:
    Field(int _height, int _width, double _food_density, double water_percentage);
    ~Field();

    Object* get(int i, int j);

    bool move_object_right(int i, int j);
    bool move_object_left(int i, int j);
    bool move_object_up(int i, int j);
    bool move_object_down(int i, int j);
};