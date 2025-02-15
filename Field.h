#pragma once
#include "Creatures.h"

class Field{
private:
    static constexpr double water_percentage = 0.3;
public:
    int height;
    int width;
    int water_density;
    static constexpr double food_density = 0.1;

    Object*** matrix;

    Field(int _height, int _width) : height{ _height }, width{ _width }, water_density{ static_cast<int>(_height * _width * water_percentage) }{
        generation();
    };

    ~Field(){
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                delete matrix[i][j];
            }
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    void generation(){
        matrix = new Object**[height];
        for (int i = 0; i < height; i++){
            matrix[i] = new Object*[width];
            for (int j = 0; j < width; j++){
                matrix[i][j] = static_cast<Object*>(nullptr);
            }
        }
    }

    Object* get(int i, int j){
        return matrix[i][j];
    }

    bool move_object_right(int i, int j){
        if (j + 1 < width && matrix[i][j+1] == nullptr){
            matrix[i][j+1] = matrix[i][j];
            matrix[i][j] = static_cast<Object*>(nullptr);
            return true;
        }
        return false;
    }
    bool move_object_left(int i, int j){
        if (j - 1 >= 0 && matrix[i][j-1] == nullptr){
            matrix[i][j-1] = matrix[i][j];
            matrix[i][j] = static_cast<Object*>(nullptr);
            return true;
        }
        return false;
    }
    bool move_object_up(int i, int j){
        if (i - 1 > 0 && matrix[i+1][j] == nullptr){
            matrix[i+1][j] = matrix[i][j];
            matrix[i][j] = static_cast<Object*>(nullptr);
            return true;
        }
        return false;
    }
    bool move_object_down(int i, int j){
        if (i + 1 < height && matrix[i-1][j] == nullptr){
            matrix[i - 1][j] = matrix[i][j];
            matrix[i][j] = static_cast<Object*>(nullptr);
            return true;
        }
        return false;
    }       
};