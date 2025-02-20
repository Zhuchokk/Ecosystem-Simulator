#pragma once

#include <cstdlib>
#include<cstdint>
//Defines
#define PROBABILITY_RANK 1000 //Used to culculate the probability. The more, the more accurate
#define GENE_LENGTH 16

//Types for id
typedef enum { FOOD, WATER, ANIMAL } OBJECT_TYPE;
typedef enum { GRASS } FOOD_TYPE;
typedef enum { RABBIT, FOX } ANIMAL_TYPE;
typedef enum { MALE, FEMALE } GENDER_TYPE;
typedef enum { SPEED_M_GROWTH, SPEED_C, MAX_AGE, AGE_OF_MATURE, AGE_OF_OLD, SPEED_OLD_GROWTH, FAST_GENE, WEIGHT, VISIBILITY_C, VISIBILITY_GENE, PREGNANCY_TIME, VISIBILITY_M_GROWTH, VISIBILITY_OLD_GROWTH
} PARAM_TYPE;


inline float CREATURES_TABLE[2][13] = {
	{0.5, 0.2, 100, 7, 55, -0.2, 0.2, 3, 3, 3, 5, 7, -3}, //RABBIT
	{ 0.5, 0.2, 70, 8, 50, -0.2, 0.2, 7, 5, 6, 7, 15, -6 } //FOX
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