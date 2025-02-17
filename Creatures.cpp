#include"Creatures.h"
#include <cstdlib>

bool Animal::roll(float chance) {
	chance *= PROBABILITY_RANK;
	return rand() % PROBABILITY_RANK < chance;
}

void Animal::Go(char s) {
	if (!roll(speed))
		return;

	switch (s) {
	case 'r': field->move_object_right(x, y); break;
	case 'l': field->move_object_left(x, y); break;
	case 'u': field->move_object_up(x, y); break;
	case 'd': field->move_object_down(x, y); break;
	}
}

void Animal::Go(uint16_t new_x, uint16_t new_y){
	if (abs(new_x - x) + abs(new_y - y) > 1) {
		throw "Go: Cannot reach these coords";
	}
	if (new_x - x == 1)
		field->move_object_right(x, y);
	else if (new_x - x == -1)
		field->move_object_left(x, y);
	else if (new_y - y == 1)
		field->move_object_up(x, y);
	else if (new_y - y == -1)
		field->move_object_down(x, y);
}

void Animal::AdjustAnimalForAge() {
	if (current_age >= CREATURES_TABLE[animal_type][AGE_OF_MATURE]) {
		speed += CREATURES_TABLE[animal_type][SPEED_M_GROWTH];
		visibility += CREATURES_TABLE[animal_type][VISIBILITY_M_GROWTH];
	}
	if (current_age >= CREATURES_TABLE[animal_type][AGE_OF_OLD]) {
		speed += CREATURES_TABLE[animal_type][SPEED_OLD_GROWTH];
		visibility += CREATURES_TABLE[animal_type][VISIBILITY_OLD_GROWTH];
	}
}

void Animal::ApplyBasicGene() {
	uint16_t res = FAST_F & basic_gene + FAST_S & basic_gene - 1; //AA -> res = 1, Aa or aA -> res = 0, aa -> res = -1
	speed += res * CREATURES_TABLE[animal_type][FAST_GENE];
	uint16_t res = VISIBILITY_F & basic_gene + VISIBILITY_S & basic_gene - 1;
	visibility += res * CREATURES_TABLE[animal_type][VISIBILITY_GENE];
}

uint16_t Animal::GenerateGene() {
	uint16_t res = rand() % ((1 << GENE_LENGTH) - 1);
}

uint16_t Animal::GenerateGene(uint16_t father_gene, uint16_t mother_gene) {
	uint16_t res = 0;
	for (int i = 0; i < GENE_LENGTH; i++) {
		bool t = rand() % 2;
		if (t)
			res |= father_gene & (1 << i);
		else
			res |= mother_gene & (1 << i);
	}
	return res;
}

void Male::ApplyMaleGene() {
	attractiveness = ATTRACT_F & male_gene + ATTRACT_S & male_gene;
}


Male::Male(Field* _field, ANIMAL_TYPE _animal_type) {
	current_age = rand() % (int)CREATURES_TABLE[_animal_type][MAX_AGE];
	thirst = rand() % (int)(CREATURES_TABLE[_animal_type][WEIGHT] / 2);
	hunger = rand() % (int)(CREATURES_TABLE[_animal_type][WEIGHT] / 2);
	repruductive_urge = rand() % 100;
	ANIMAL_TYPE animal_type = _animal_type;
	GENDER_TYPE gender_type = MALE;
	basic_gene = GenerateGene();


	speed = CREATURES_TABLE[_animal_type][SPEED_C];
	visibility = CREATURES_TABLE[_animal_type][VISIBILITY_C];
	AdjustAnimalForAge();

	ApplyBasicGene();
	ApplyMaleGene();
	
	//TODO: Randomly put an object
	/*Field* field = _field;
	uint16_t w = field.width;
	uint16_t h = field.height;
	x;
	y;*/
	

}