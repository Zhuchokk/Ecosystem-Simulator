#include "Field.h"
#include <iostream>

Field::Field(uint16_t _height, uint16_t _width, double _food_procentage, double water_percentage) // food and water procentage expected from 0 to 100
    : height(_height),
    width(_width),
    water_density(static_cast<int>(_height * _width * water_percentage / 100)),
    food_density(static_cast<int>(_height * _width * _food_procentage) / 100),
    food_number(0)
{
    generation();
}

Field::~Field(){
    for (uint16_t i = 0; i < height; i++){
        for (uint16_t j = 0; j < width; j++){
            delete matrix[i][j];
        }
        delete[] matrix[i];
    }
    delete[] matrix;
}

void Field::generation(){
    matrix = new Object**[height];
    for (uint16_t i = 0; i < height; i++){
        matrix[i] = new Object*[width];
        for (uint16_t j = 0; j < width; j++){
            matrix[i][j] = static_cast<Object*>(nullptr);
        }
    }

    int count_water_ceils = 0;  
    while (count_water_ceils < water_density){
        uint16_t min_r = 1;
        uint16_t max_r = std::max(1, std::min(width, height) / 10);
        uint16_t r =  min_r + rand() % (max_r - min_r + 1);
        uint16_t x = rand() % width, y = rand() % height;
        count_water_ceils += water_maker(x, y, r);
    }

    food_maker();
}

int Field:: water_maker(uint16_t x, uint16_t y, uint16_t r){
    int water_count = 0;
    for (uint16_t i = x - r; i < r + x + 1; i++){
        for (uint16_t j = y - r; j < r + y + 1; j++){
            if ((i >= 0 && i < width) && (j >= 0 && j < height) && ((x - i) * (x - i) + (y - j) * (y - j) <= r * r) && matrix[i][j] == nullptr){
                matrix[i][j] = new Water();
                water_count++;
            }
        }
    }
    return water_count;
}

void Field::food_maker(){
    while (food_density > food_number){
        uint16_t y = rand() % height;
        uint16_t x = rand() % width;
        if (matrix[y][x] == nullptr){
            food_number++;
            Food* food = new Food();
            food->food_type = GRASS;
            food->food_value = 1;
            matrix[y][x] = food;
        }
    }
}

uint16_t Field::get_width(){
    return width;
}

uint16_t Field::get_height(){
    return height;
}


void Field::del(uint16_t y, uint16_t x){
    delete matrix[y][x];
    matrix[y][x] = nullptr;
}

Object* Field::get(uint16_t i, uint16_t j){
    return matrix[i][j];
}

bool Field::set(uint16_t i, uint16_t j, Object* obj){
    if (!matrix[i][j]){
        matrix[i][j] = obj;
        return true;
    }
    return false;
}

bool Field::move_object_right(uint16_t y, uint16_t x){
    if (x + 1 < width && matrix[y][x+1] == nullptr) {
        matrix[y][x+1] = matrix[y][x];
        matrix[y][x] = static_cast<Object*>(nullptr);
        return true;
    }
    return false;
}

bool Field::move_object_left(uint16_t y, uint16_t x){
    if (x - 1 >= 0 && matrix[y][x-1] == nullptr) {
        matrix[y][x-1] = matrix[y][x];
        matrix[y][x] = static_cast<Object*>(nullptr);
        return true;
    }
    return false;
}

bool Field::move_object_up(uint16_t y, uint16_t x){
    if (y - 1 >= 0 && matrix[y-1][x] == nullptr) {
        matrix[y-1][x] = matrix[y][x];
        matrix[y][x] = static_cast<Object*>(nullptr);
        return true;
    }
    return false;
}

bool Field::move_object_down(uint16_t y, uint16_t x){
    if (y+ 1 < height && matrix[y+1][x] == nullptr) {
        matrix[y+1][x] = matrix[y][x];
        matrix[y][x] = static_cast<Object*>(nullptr);
        return true;
    }
    return false;
}