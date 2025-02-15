#pragma once

//Types for id
typedef enum{ FOOD, WATER,ANIMAL} OBJECT_TYPE;
typedef enum { GRASS } FOOD_TYPE;

//Main class for all creatures
class Object {
public:
	OBJECT_TYPE obj_type;
};

class Animal:public Object {
};

class Food: public Object {
	FOOD_TYPE food_type;
};

class Water : public Object {
};