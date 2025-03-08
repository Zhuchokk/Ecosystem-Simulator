#pragma once

#include <cstdlib>
#include<cstdint>
//Defines
#define PROBABILITY_RANK 1000 //Used to culculate the probability. The more, the more accurate
#define GENE_LENGTH 16
#define KILOS_PER_FOOD_VALUE 1
#define KILOS_PER_WATER_VALUE 5
#define TOO_FAST_CHILDBIRTH 0.8
#define TOO_SLOW_CHILDBIRTH 0.9
#define CHILD_THIRST_COEF 0.7 // how much water child initially has
#define CHILD_HUNGER_COEF 0.7  // how much food
#define FPS 2 // how many ticks per second
#define MAX_VISIBILITY_RAD 15

//Types for id
typedef enum { FOOD, WATER, ANIMAL } OBJECT_TYPE;
typedef enum { GRASS } FOOD_TYPE;
typedef enum { RABBIT, FOX } ANIMAL_TYPE;
typedef enum { MALE, FEMALE } GENDER_TYPE;
typedef enum { SPEED_M_GROWTH, SPEED_C, MAX_AGE, AGE_OF_MATURE, AGE_OF_OLD, SPEED_OLD_GROWTH, FAST_GENE, WEIGHT, VISIBILITY_C, VISIBILITY_GENE, PREGNANCY_TIME,
	VISIBILITY_M_GROWTH, VISIBILITY_OLD_GROWTH, PREGNANCY_GENE, STRENGTH_C, STRENGTH_M_GROWTH, STRENGTH_OLD_GROWTH, AGILITY_C, AGILITY_M_GROWTH, AGILITY_OLD_GROWTH, 
	LIFESPAN, CHILDREN_COUNT
} PARAM_TYPE;


inline float CREATURES_TABLE[2][22] = {
	{0.5, 0.2, 100, 7, 55, -0.2, 0.2, 3, 2, 2, 5, 3, -2, 2, 0, 3, -1, 6, 4, -3, 70, 6}, //RABBIT
	{ 0.5, 0.2, 70, 8, 50, -0.2, 0.2, 7, 3, 3, 7, 7, -2, 3, 2, 5, -2, 3, 4, -3, 85, 4} //FOX
};


//Main class for all creatures
class Object {
public:
	OBJECT_TYPE obj_type;
	uint16_t food_value;
	uint16_t water_value; 
};

class Food : public Object {
public:
	FOOD_TYPE food_type;
};

class Water : public Object {
};

struct Edge{
	uint8_t who_eats;
	uint8_t who_is_eaten;
	uint8_t is_plant;

	Edge(uint8_t who_eats, uint8_t who_is_eaten, uint8_t is_plant) : who_eats(who_eats), who_is_eaten(who_is_eaten), is_plant(is_plant) {};
};