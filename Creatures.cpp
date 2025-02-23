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

void Animal::Ramble() {
	uint8_t res = rand() % 4;
	switch (res)
	{
	case 0: Go('l'); break;
	case 1: Go('r'); break;
	case 2: Go('u'); break;
	case 3: Go('d'); break;
	}
}

void Animal::ShuffleBasicParameters() {
	//parameters set
	current_age = rand() % (int)CREATURES_TABLE[animal_type][MAX_AGE];
	thirst = rand() % (int)(CREATURES_TABLE[animal_type][WEIGHT] / 2);
	hunger = rand() % (int)(CREATURES_TABLE[animal_type][WEIGHT] / 2);
	repruductive_urge = rand() % 50;
	speed = CREATURES_TABLE[animal_type][SPEED_C];
	visibility = CREATURES_TABLE[animal_type][VISIBILITY_C];
	AdjustAnimalForAge();

	food_value = CREATURES_TABLE[animal_type][WEIGHT] / KILOS_PER_FOOD_VALUE;
	water_value = CREATURES_TABLE[animal_type][WEIGHT] / KILOS_PER_WATER_VALUE;

	//genes set
	basic_gene = GenerateGene();
	ApplyBasicGene();

	//Set location

	
	uint16_t w = field->get_width();
	uint16_t h = field->get_height();
	do {
		x = rand() % w;
		y = rand() % h;
	} while (!field->set(x, y, this));
}

void Animal::ApplyChildParameters(){
	current_age = 0;
	thirst = (CREATURES_TABLE[animal_type][WEIGHT] / 2) * CHILD_THIRST_COEF;
	hunger = (CREATURES_TABLE[animal_type][WEIGHT] / 2) * CHILD_HUNGER_COEF;
	repruductive_urge = 0;
	speed = CREATURES_TABLE[animal_type][SPEED_C];
	visibility = CREATURES_TABLE[animal_type][VISIBILITY_C];
	food_value = CREATURES_TABLE[animal_type][WEIGHT] / KILOS_PER_FOOD_VALUE;
	water_value = CREATURES_TABLE[animal_type][WEIGHT] / KILOS_PER_WATER_VALUE;
}


void Male::ApplyMaleGene() {
	attractiveness = ATTRACT_F & male_gene + ATTRACT_S & male_gene;
}


Male::Male(Field* _field, ANIMAL_TYPE _animal_type) {
	animal_type = _animal_type;
	gender_type = MALE;
	field = _field;
	ShuffleBasicParameters();
	
	ApplyMaleGene();
}

Male::Male(Male* father, Female* mother) {
	animal_type = father->animal_type;
	gender_type = MALE;
	field = father->field;
	ApplyChildParameters();

	//genes set
	basic_gene = GenerateGene(father->basic_gene, mother->GetBasicGene());
	ApplyBasicGene();
	male_gene = father->male_gene;
	ApplyMaleGene();

	//Set location TODO
}

void Female::ApplyFemaleGene() {
	preg_quality = PREGNANCY_F & female_gene + PREGNANCY_S & female_gene;
}

Female::Female(Field* _field, ANIMAL_TYPE _animal_type) {
	animal_type = _animal_type;
	gender_type = FEMALE;
	field = _field;
	ShuffleBasicParameters();

	ApplyFemaleGene();
}
Female::Female(Male* father, Female* mother) {
	animal_type = mother->animal_type;
	gender_type = MALE;
	field = mother->field;
	ApplyChildParameters();

	//genes set
	basic_gene = GenerateGene(father->GetBasicGene(), mother->basic_gene);
	ApplyBasicGene();
	female_gene = mother->female_gene;
	ApplyFemaleGene();

	//Set location TODO
}