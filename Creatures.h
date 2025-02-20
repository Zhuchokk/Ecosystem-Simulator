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
	uint16_t visibility;
	uint16_t basic_gene; // bits: 1 - A(Dominant), 0 - a(Recessive)
	ANIMAL_TYPE animal_type;
	GENDER_TYPE gender_type;
	uint16_t x;
	uint16_t y;
	static Field* field;

	void Ramble();
	void GoToTarget(uint16_t x, uint16_t y);
	uint16_t* CheckForTarget(); // Returns {x, y} TODO: ADD PARAMETERS
	uint16_t* GetOut(uint16_t x, uint16_t y);
	bool roll(float chance); //Returns the probability of chance drop
	void Go(char s); //The side of moving: r, l, d, u
	void Go(uint16_t new_x, uint16_t new_y); //coords to where animal will move
	uint16_t GenerateGene();
	uint16_t GenerateGene(uint16_t father_gene, uint16_t mother_gene);
	void AdjustAnimalForAge(); // Changes speed and visibility of animal, according it's age.
	void ApplyBasicGene();

public:

	ANIMAL_TYPE who() { return animal_type; };
	uint16_t* where();
	void BasicLive(); // Main logic here
};

class Female : public Animal {
	static const uint16_t PREGNANCY_F = 0x01;
	static const uint16_t PREGNANCY_S = 0x02;
	static const uint16_t CONSENT_F = 0x01;
	static const uint16_t CONSENT_S = 0x02;

	uint16_t female_gene;
	uint16_t cur_preg_time;
	
	
	bool RecieveMateRequest();
	void GiveBirth();
	void ApplyFemaleGene();
public:
	bool hired;

	Female();
	~Female();

	void Live();
};

class Male : public Animal {
	static const uint16_t ATTRACT_F = 0x01;
	static const uint16_t ATTRACT_S = 0x02;

	uint16_t male_gene;
	uint16_t attractiveness; //0 - ugly, 1 - ok, 2 - beautiful
	Female* partner;
	bool SendMateRequest(Female* partner); 
	void ApplyMaleGene();

	uint16_t* MCheckForTarget(); //Finding predotors or partners
public:
	Male(Field* _field, ANIMAL_TYPE _animal_type); //for initialization of the field, randomly generates all
	Male(Male* father, Animal* mother); //For birth
	~Male();

	void Live();
};