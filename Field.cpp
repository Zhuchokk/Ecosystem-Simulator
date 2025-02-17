#include "Field.h"
#include <iostream>

Field::Field(int _height, int _width, double _food_density, double water_percentage) 
    : height(_height),
    width(_width),
    water_density(static_cast<int>(_height * _width * water_percentage)),
    food_density(_food_density)
{
    generation();
}

Field::~Field(){
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            delete matrix[i][j];
        }
        delete[] matrix[i];
    }
    delete[] matrix;
}

void Field::generation(){
    matrix = new Object**[height];
    for (int i = 0; i < height; i++){
        matrix[i] = new Object*[width];
        for (int j = 0; j < width; j++){
            matrix[i][j] = static_cast<Object*>(nullptr);
        }
    }

    int count_water_ceils = 0;
    srand(time(0));
    while (count_water_ceils < water_density){
        int min_r = 1;
        int max_r = std::max(1, std::min(width, height) / 10);
        int r =  min_r + rand() % (max_r - min_r + 1);
        int x = rand() % width, y = rand() % height;
        count_water_ceils += water_maker(x, y, r);
    }
}

int Field:: water_maker(int x, int y, int r){
    int water_count = 0;
    for (int i = x - r; i < r + x + 1; i++){
        for (int j = y - r; j < r + y + 1; j++){
            if ((i >= 0 && i < width) && (j >= 0 && j < height) && ((x - i) * (x - i) + (y - j) * (y - j) <= r * r) && matrix[i][j] == nullptr){
                matrix[i][j] = new Water();
                water_count++;
            }
        }
    }
    return water_count;
}

Object* Field::get(int i, int j){
    return matrix[i][j];
}

bool Field::move_object_right(int i, int j){
    if (j + 1 < width && matrix[i][j+1] == nullptr) {
        matrix[i][j+1] = matrix[i][j];
        matrix[i][j] = static_cast<Object*>(nullptr);
        return true;
    }
    return false;
}

bool Field::move_object_left(int i, int j){
    if (j - 1 >= 0 && matrix[i][j-1] == nullptr) {
        matrix[i][j-1] = matrix[i][j];
        matrix[i][j] = static_cast<Object*>(nullptr);
        return true;
    }
    return false;
}

bool Field::move_object_up(int i, int j){
    if (i - 1 >= 0 && matrix[i-1][j] == nullptr) {
        matrix[i-1][j] = matrix[i][j];
        matrix[i][j] = static_cast<Object*>(nullptr);
        return true;
    }
    return false;
}

bool Field::move_object_down(int i, int j){
    if (i + 1 < height && matrix[i+1][j] == nullptr) {
        matrix[i+1][j] = matrix[i][j];
        matrix[i][j] = static_cast<Object*>(nullptr);
        return true;
    }
    return false;
}