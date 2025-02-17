#pragma once

#include<cstdint>
#include"Commons.h"
#include"Field.h"


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
	float speed;
	uint16_t basic_visibility;
	uint16_t basic_gene;
	ANIMAL_TYPE animal_type;
	GENDER_TYPE gender_type;
	uint16_t x;
	uint16_t y;
	Field* field;

	void Ramble();
	void GoToTarget(uint16_t x, uint16_t y);
	uint16_t* CheckForTarget(); // Returns {x, y} TODO: ADD PARAMETERS
	bool roll(float chance); //Returns the probability of chance drop
	void Go(char s); //The side of moving: r, l, d, u
	void Go(uint16_t new_x, uint16_t new_y); //coords to where animal will move


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
	static const uint16_t CONSENT_F = 0x01;
	static const uint16_t CONSENT_S = 0x02;

	uint16_t female_gene;
	uint16_t cur_preg_time;


	bool RecieveMateRequest();
	void GiveBirth();
public:
	Female();
	~Female();

};

