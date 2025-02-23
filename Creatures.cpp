#include"Creatures.h"
#include <cstdlib>

uint16_t* Animal::CheckForTarget(){
	uint16_t max_urge = 0;
	uint16_t predator_urge = 5;
	uint16_t food_urge, water_urge;
	if(thirst > hunger){
		water_urge = 4, food_urge = 0;
	} else{
		water_urge = 0, food_urge = 4;
	}

	uint16_t min_x = x - visibility ? x - visibility >= 0 : 0, max_x = x + visibility ? x + visibility <= field->get_width() : field ->get_width();
	uint16_t min_y = y - visibility ? y - visibility >= 0 : 0, max_y = y + visibility ? y + visibility <= field->get_height() : field->get_height();

	uint16_t* target_coord = new uint16_t[2];

	for (int i = min_y; min_y < max_y; i++){
		for (int j = min_x; min_x < max_x; j++){
			uint16_t target_priority;
			Object* target = field->get(i, j);
			if (target->obj_type == WATER){
				target_priority = water_urge;
			} else if(target->obj_type == FOOD){
				target_priority = food_urge;
			} else{
				Animal* animal = (Animal*)target;
				if (animal->animal_type == FOX){
					target_priority = predator_urge;
				}
			}
			if (target_priority > max_urge){
				max_urge = target_priority;
				target_coord[0] = i, target_coord[1] = j;
			}
		}
		return target_coord;
	}
}

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

void Animal::BasicLive(){
	uint16_t* target = CheckForTarget();
	if (!target){
		Ramble();
		return ;
	}
	else if(field->get(target[0], target[0])->obj_type == ANIMAL){
		GetOut(target[0], target[1]);
	}
	else{
		if ((x - target[0])*(x - target[0]) + (y - target[1]) * (y - target[1]) == 1){
			if (field->get(target[0], target[1])->obj_type == FOOD){
				uint16_t food = hunger - field->get(target[0], target[1])->food_value;
				hunger = hunger - food ? hunger - food > 0 : 0;
				field->del(target[0], target[1]);
			}
			else{
				uint16_t water = CREATURES_TABLE[animal_type][WEIGHT];
				thirst = thirst - water ? thirst - water > 0 : 0;
			}
		} else {
			GoToTarget(target[0], target[1]);
		}
	}
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

void Male:: Live(){
	AdjustAnimalForAge();
	if (partner || (repruductive_urge > thirst && repruductive_urge > hunger)){
		uint16_t* target = MCheckForTarget();
		if (!target && !partner){
			Ramble();
			return ;
		}
		Animal* animal = (Animal*)field->get(target[0], target[1]);
		if (animal->who() == FOX){
			if(partner){
				partner->hired = false;
				partner = nullptr;
			}
			GetOut(target[0], target[1]);
		} else if(partner){
			if (partner->hired){
				uint16_t* partner_coord = partner->where();
				if ((x - target[0])*(x - target[0]) + (y - target[1]) * (y - target[1]) > 1){
					GoToTarget(partner_coord[0], partner_coord[1]);
				}
			} else{
				partner = nullptr;
				Ramble();
			}
		} else{
			partner = (Female*)(Animal*)field->get(target[0], target[1]);
			if (SendMateRequest(partner)){
				GoToTarget(target[0], target[1]);
			} else{
				Ramble();
			}
		}

	} else{
		BasicLive();
	}
}

void Female:: Live(){
	AdjustAnimalForAge();
	if (hired){
		uint16_t* target = CheckForTarget();
		if (field->get(target[0], target[1])->obj_type == ANIMAL){
			Animal* animal = (Animal*)field->get(target[0], target[1]);
			if (animal->who() == FOX){
				hired = false;
				GetOut(target[0], target[1]);
			}
		}
		return ;
	} else{
		BasicLive();
	}
}