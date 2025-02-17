#pragma once

#include <cstdlib>
#include<cstdint>
//Defines
#define PROBABILITY_RANK 1000 //Used to culculate the probability. The more, the more accurate

//Types for id
typedef enum { FOOD, WATER, ANIMAL } OBJECT_TYPE;
typedef enum { GRASS } FOOD_TYPE;
typedef enum { RABBIT, FOX } ANIMAL_TYPE;
typedef enum { MALE, FEMALE } GENDER_TYPE;
typedef enum { SPEED_M_GROWTH, SPEED_C, MAX_AGE, AGE_OF_MATURE, AGE_OF_OLD, SPEED_OLD_GROWTH, FAST_GENE, WEIGHT, VISIBILITY, VISIBILITY_GENE, PREGNANCY_TIME } PARAM_TYPE;


inline float CREATURES_TABLE[2][11] = {
	{0.5, 0.2, 100, 7, 55, -0.2, 0.2, 3, 10, 3, 5}, //RABBIT
	{ 0.5, 0.2, 70, 8, 50, -0.2, 0.2, 7, 20, 6, 7 } //FOX
};


//Main class for all creatures
class Object {
public:
	OBJECT_TYPE obj_type;
	uint16_t food_value;
	uint16_t water_value; 
};

class Food : public Object {
	FOOD_TYPE food_type;
};

class Water : public Object {
};