#pragma once

#include<cstdint>

//Types for id
typedef enum{ FOOD, WATER,ANIMAL} OBJECT_TYPE;
typedef enum { GRASS } FOOD_TYPE;
typedef enum {RABBIT, FOX} ANIMAL_TYPE;
typedef enum {SPEED_M_GROWTH, SPEED_C, MAX_AGE, AGE_OF_MATURE, AGE_OF_OLD, SPEED_OLD_GROWTH, FAST_GENE, WEIGHT, VISIBILITY, VISIBILITY_GENE} PARAM_TYPE;


inline float CREATURES_TABLE[2][10] = {
	{0.5, 0.2, 100, 7, 55, -0.2, 0.2, 3, 10, 3}, //RABBIT
	{ 0.5, 0.2, 70, 8, 50, -0.2, 0.2, 7, 20, 6 } //FOX
};


//Main class for all creatures
class Object {
public:
	OBJECT_TYPE obj_type;
	uint16_t food_value;
	uint16_t water_value;
};


class Animal:public Object {
protected:
	static const uint16_t FAST_F = 0x01; // 00000001
	static const uint16_t FAST_S = 0x02; // 00000010
	static const uint16_t VISIBILITY_F = 0x04; // 00000100
	static const uint16_t VISIBILITY_S = 0x08; // 00001000

	uint16_t current_age;
	uint16_t thirst; 
	uint16_t hunger; 
	uint16_t repruductive_urge;
	float basic_speed;
	uint16_t basic_visibility;
	uint16_t basic_gene;
	ANIMAL_TYPE animal_type;

	void Ramble();
	void GoToTarget(uint16_t x, uint16_t y);
	uint16_t* CheckForTarget(); // Returns {x, y} TODO: ADD PARAMETERS


public:

	ANIMAL_TYPE who() { return animal_type; };
	void Live(); // Main logic here
};

class Male : Animal {
	static const uint16_t ATTRACT_F = 0x01;
	static const uint16_t ATTRACT_S = 0x02;

	uint16_t male_gene;

	bool SendMateRequest(); 

public:
	Male();
	~Male();
};

class Female : Animal {
	static const uint16_t PREGNANCY_F = 0x01;
	static const uint16_t PREGNANCY_S = 0x02;

	uint16_t female_gene;

	bool RecieveMateRequest();
public:
	Female();
	~Female();

};

class Food: public Object {
	FOOD_TYPE food_type;
};

class Water : public Object {
};