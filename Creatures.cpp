#include"Creatures.h"
#include <cstdlib>
#include <unordered_set>
#include<bitset>
#include<queue>
#include <iostream>
#include <exception>

int hash(uint8_t who_eats, uint8_t who_is_eaten, uint8_t is_plant) {
	return (is_plant << 16) + (who_eats << 7) + who_is_eaten;
}
std:: unordered_set<int> CHAINING;

uint16_t HeuristicManhattan(uint16_t x_tar, uint16_t y_tar, uint16_t x, uint16_t y) {
	return abs(x_tar - x) + abs(y_tar - y);
}

bool Compare::operator() (std::pair<uint16_t, uint16_t> a, std::pair<uint16_t, uint16_t> b) {
	if (HeuristicManhattan(a.first, a.second, x, y) > HeuristicManhattan(b.first, b.second, x, y)) { // with the smallest distance will be taken first
		return true;
	}
	return false;
}

void Animal::GoToTarget(uint16_t x, uint16_t y) {
	if (abs(this->x - x) + abs(this->y - y) <= 1) {
		return; //checking wether an object is reachable
	}
	if (next_step != -1 && next_step < 2 * MAX_VISIBILITY_RAD && path[next_step] != -1 && IsPathValid()) {
		Go(path[next_step]); // using existing path
		next_step++;
		return;
	}
	
	std::bitset<(2 * MAX_VISIBILITY_RAD + 1) * (2 * MAX_VISIBILITY_RAD + 1)> visited; //matrix (n + 1) * (n + 1)
	std::vector<uint16_t> parent((2 * MAX_VISIBILITY_RAD + 1) * (2 * MAX_VISIBILITY_RAD + 1), -1);
	auto get_index = [](uint16_t x, uint16_t y, uint16_t myx, uint16_t myy) {return (x - myx + MAX_VISIBILITY_RAD) * (MAX_VISIBILITY_RAD * 2 + 1) + y - myy + MAX_VISIBILITY_RAD; };
	std::priority_queue<std::pair<uint16_t, uint16_t>, std::vector<std::pair<uint16_t, uint16_t>>, Compare> q; 
	bool found = 0;
	q.push({x, y});
	Compare::x = x;
	Compare::y = y;
	

	while (!q.empty()) {
		std::pair<uint16_t, uint16_t> cur = q.top();
		q.pop();
		if (cur.first == x && cur.second == y) { //target found
			found = 1;
			break;
		} 
		visited[(cur.first - x + MAX_VISIBILITY_RAD) * (MAX_VISIBILITY_RAD * 2 + 1) + cur.second - y + MAX_VISIBILITY_RAD] = 1;
		if (cur.first + 1 < field->get_width() && cur.first + 1 <= x + visibility && visited[get_index(cur.first + 1, cur.second, x, y)] == 0)  {
			q.push({ x + 1, y });
			parent[get_index(cur.first + 1, cur.second, x, y)] = parent[get_index(cur.first, cur.second, x, y)];
		}
		if (cur.first - 1 >= 0 && cur.first - 1 >= x - visibility && visited[get_index(cur.first - 1, cur.second, x, y)] == 0) {
			q.push({ x - 1, y });
			parent[get_index(cur.first - 1, cur.second, x, y)] = parent[get_index(cur.first, cur.second, x, y)];
		}
		if (cur.second + 1 < field->get_height() && cur.second + 1 <= y + visibility && visited[get_index(cur.first, cur.second + 1, x, y)] == 0) {
			q.push({ x, y + 1});
			parent[get_index(cur.first, cur.second + 1, x, y)] = parent[get_index(cur.first, cur.second, x, y)];
		}
		if (cur.second - 1 >= 0 && cur.second - 1 >= y - visibility && visited[get_index(cur.first, cur.second - 1, x, y)] == 0) {
			q.push({ x, y - 1});
			parent[get_index(cur.first, cur.second - 1, x, y)] = parent[get_index(cur.first, cur.second, x, y)];
		}
	}
	if (found) {
		int step = get_index(x, y, x, y); //target
		next_step = 2 * MAX_VISIBILITY_RAD - 1; // length of path array - 1
		while (parent[step] != -1) {
			uint16_t y_offset = parent[step] % (2 * MAX_VISIBILITY_RAD + 1) - MAX_VISIBILITY_RAD;
			uint16_t x_offset = (parent[step] - y_offset - MAX_VISIBILITY_RAD) / (2 * MAX_VISIBILITY_RAD + 1) - MAX_VISIBILITY_RAD;
			//inverse offsets, going from end of path to the beginning
			if (x_offset == 1) {
				path[next_step] = 'l';
				next_step--;
			}
			else if (x_offset == -1) {
				path[next_step] = 'r';
				next_step--;
			}
			else if (y_offset == 1) {
				path[next_step] = 'u';
				next_step--;
			}
			else if (y_offset == -1) {
				path[next_step] = 'd';
				next_step--;
			}
			else {
				std::cout << "Something wrong in BFS";
				throw "Error in BFS";
			}
			step = parent[step];
		}
		next_step++;
		if (step != get_index(this->x, this->y, x, y)) { //Checking that we reached current position of animal
			std::cout << "Reached Wrong position of animal in BFS";
			throw "Wrong position in BFS";
		}
	}
	else {
		//target isn't reachable
		std::cout << "Target isn't reachable";
		throw "Can't find path in BFS";
	}
	//After creating the path, go throw
	Go(path[next_step]);
	next_step++;
}

bool Animal::IsPathValid() {
	if (next_step == -1) { // we don't have path
		return false;
	}
	for (uint8_t i = next_step; i < 2 * MAX_VISIBILITY_RAD; i++) {
		if (path[i] == -1) // the path ended
			break;
		if (path[i] == 'u' && field->get(y - 1, x) == nullptr || path[i] == 'd' && field->get(y + 1, x) == nullptr ||
			path[i] == 'l' && field->get(y, x - 1) == nullptr || path[i] == 'r' && field->get(y, x + 1) == nullptr) {
			return false;
		}
	}
	return true;
}

void Animal::GetOut(uint16_t _y, uint16_t _x) {
	uint16_t dif_x = this->x - _x, dif_y = this->y - _y;
	if (abs(dif_x) > abs(dif_y)) {
		if (dif_x > 0) {
			Go('r');
		}
		else {
			Go('l');
		}
	} else{
		if (dif_y > 0) {
			Go('d');
		}
		else {
			Go('u');
		}
	}
}

uint16_t* Animal::CheckForTarget(){
	uint16_t max_urge = 0;
	uint16_t predator_urge = 5;
	uint16_t food_urge, water_urge;
	if(thirst_urge > hunger_urge){
		water_urge = 4, food_urge = 0;
	} else{
		water_urge = 0, food_urge = 4;
	}

	uint16_t min_x = x - visibility >= 0 ? x - visibility : 0, \
			 max_x = x + visibility <= field->get_width() ? x + visibility : field ->get_width();
	uint16_t min_y = y - visibility >= 0 ? y - visibility : 0, \
	 		 max_y = y + visibility <= field->get_height() ? y + visibility : field->get_height();

	uint16_t* target_coord = new uint16_t[2];

	for (int i = min_y; min_y < max_y; i++){
		for (int j = min_x; min_x < max_x; j++){
			uint16_t target_priority;
			Object* target = field->get(i, j);
			if (target->obj_type == WATER){
				target_priority = water_urge;
			} else if(target->obj_type == FOOD){
				Food* food = (Food*)target;
				Edge edge = Edge(static_cast<uint8_t>(animal_type), static_cast<uint8_t>(food->food_type), 1);
				if (CHAINING.find(hash(edge.who_eats, edge.who_is_eaten, edge.is_plant)) != CHAINING.end()){
					target_priority = food_urge;
				}

			} else{
				Animal* animal = (Animal*)target;
				Edge edge = Edge(static_cast<uint8_t>(animal->who()), static_cast<uint8_t>(this->animal_type), 0);
				if (CHAINING.find(hash(edge.who_eats, edge.who_is_eaten, edge.is_plant)) != CHAINING.end()){
					target_priority = predator_urge;
				} else if(CHAINING.find(hash(edge.who_is_eaten, edge.who_eats, edge.is_plant)) != CHAINING.end()){
					target_priority = food_urge;
				}
			}
			if (target_priority > max_urge){
				max_urge = target_priority;
				target_coord[0] = i, target_coord[1] = j;
			} else if (target_priority == max_urge){
				if (abs(i - x) + abs(j - y) < abs(target_coord[0] - x) + abs(target_coord[1] - y)){
					target_coord[0] = i, target_coord[1] = j;
				}
			}
		}
	}
	return target_coord;
}

bool Animal::roll(float chance) {
	chance *= PROBABILITY_RANK;
	return rand() % PROBABILITY_RANK < chance;
}

void Animal::Go(char s) {
	//Deduct some water and food for movement
	thirst -= speed * CREATURES_TABLE[animal_type][WEIGHT] * 0.02;
	hunger -= speed * CREATURES_TABLE[animal_type][WEIGHT] * 0.08;
	AdjustUrge();

	if (!roll(speed))
		return;
	
	switch (s) {
	case 'r': field->move_object_right(y, x); break;
	case 'l': field->move_object_left(y, x); break;
	case 'u': field->move_object_up(y, x); break;
	case 'd': field->move_object_down(y, x); break;
	}
}

void Animal::Go(uint16_t new_x, uint16_t new_y){
	if (abs(new_x - x) + abs(new_y - y) > 1) {
		throw "Go: Cannot reach these coords";
	}
	if (new_x - x == 1)
		field->move_object_right(y, x);
	else if (new_x - x == -1)
		field->move_object_left(y, x);
	else if (new_y - y == 1)
		field->move_object_down(y, x);
	else if (new_y - y == -1)
		field->move_object_up(y, x);
}

void Animal::AdjustAnimalForAge() {
	if (current_age >= CREATURES_TABLE[animal_type][AGE_OF_MATURE]) {
		speed += CREATURES_TABLE[animal_type][SPEED_M_GROWTH];
		visibility += CREATURES_TABLE[animal_type][VISIBILITY_M_GROWTH];
		agility += CREATURES_TABLE[animal_type][AGILITY_M_GROWTH];
		strength += CREATURES_TABLE[animal_type][STRENGTH_M_GROWTH];
	}
	if (current_age >= CREATURES_TABLE[animal_type][AGE_OF_OLD]) {
		speed += CREATURES_TABLE[animal_type][SPEED_OLD_GROWTH];
		visibility += CREATURES_TABLE[animal_type][VISIBILITY_OLD_GROWTH];
		agility += CREATURES_TABLE[animal_type][AGILITY_OLD_GROWTH];
		strength += CREATURES_TABLE[animal_type][STRENGTH_OLD_GROWTH];
	}
}

void Animal::AdjustUrge(){
	thirst_urge = static_cast<uint16_t>(100 - thirst * 2 * 100 / CREATURES_TABLE[animal_type][WEIGHT]);
	hunger_urge = static_cast<uint16_t>(100 - hunger * 2 * 100 / CREATURES_TABLE[animal_type][WEIGHT]);  
}

void Animal::ApplyBasicGene() {
	uint16_t res = FAST_F & basic_gene + FAST_S & basic_gene - 1; //AA -> res = 1, Aa or aA -> res = 0, aa -> res = -1
	speed += res * CREATURES_TABLE[animal_type][FAST_GENE];
	res = VISIBILITY_F & basic_gene + VISIBILITY_S & basic_gene - 1;
	visibility += res * CREATURES_TABLE[animal_type][VISIBILITY_GENE];
}

uint16_t* Male::MCheckForTarget() {
	uint16_t max_urge = 0;
	uint16_t predator_urge = 5;
	uint16_t libido = 4;
  
	uint16_t min_x = x - visibility >= 0 ? x - visibility : 0,
			 max_x = x + visibility <= field->get_width() ? x + visibility : field ->get_width();
	uint16_t min_y = y - visibility >= 0 ? y - visibility : 0,
			 max_y = y + visibility <= field->get_height() ? y + visibility : field->get_height();
  
	uint16_t* target_coord = new uint16_t[2];
  
	for (int i = min_y; min_y < max_y; i++){
		for (int j = min_x; min_x < max_x; j++){
			uint16_t target_priority = 0;
			Object* target = field->get(i, j);
			if (field->get(i, j)->obj_type == ANIMAL){
				Animal* animal = (Animal*)target;
				Edge edge = Edge(static_cast<uint8_t>(animal->who()), static_cast<uint8_t>(animal_type), 0);
				if (CHAINING.find(hash(edge.who_eats, edge.who_is_eaten, edge.is_plant)) != CHAINING.end()){
					target_priority = predator_urge;
				} else if(animal->who() == animal_type && animal->get_gender() == FEMALE){
					Female* female = (Female*)animal;
					if (!female->pregnant && !female->hired){
						target_priority = libido;
					}
				}
			}
			if (max_urge < target_priority){
				max_urge = target_priority;
				target_coord[0] = i, target_coord[1] = j;
			} else if (max_urge == target_priority){
				if (abs(y - i) + abs(x - j) < abs(y - target_coord[0]) + abs(x - target_coord[1])){
					target_coord[0] = i, target_coord[1] = j;
				}
			}
	  	}
	}
	return target_coord;
}

void Male::Mating(){
	partner->Mating(basic_gene, male_gene);
	partner = nullptr;
}

uint16_t Animal::GenerateGene() {
	uint16_t res = rand() % ((1 << GENE_LENGTH) - 1);
	return res;
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
	//set path
	path[0] = -1;
	next_step = -1;

	//parameters set
	current_age = rand() % (int)CREATURES_TABLE[animal_type][MAX_AGE];
	thirst = rand() % (int)(CREATURES_TABLE[animal_type][WEIGHT] / 2);
	hunger = rand() % (int)(CREATURES_TABLE[animal_type][WEIGHT] / 2);
	AdjustUrge();
	repruductive_urge = rand() % 50;
	speed = CREATURES_TABLE[animal_type][SPEED_C];
	visibility = CREATURES_TABLE[animal_type][VISIBILITY_C];
	agility = CREATURES_TABLE[animal_type][AGILITY_C];
	strength = CREATURES_TABLE[animal_type][STRENGTH_C];
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
	} while (!field->set(y, x, this));
}

void Animal::ApplyChildParameters(){
	//set path
	path[0] = -1;
	next_step = -1;

	//parameters
	current_age = 0;
	thirst = (CREATURES_TABLE[animal_type][WEIGHT] / 2) * CHILD_THIRST_COEF;
	hunger = (CREATURES_TABLE[animal_type][WEIGHT] / 2) * CHILD_HUNGER_COEF;
	AdjustUrge();
	repruductive_urge = 0;
	speed = CREATURES_TABLE[animal_type][SPEED_C];
	visibility = CREATURES_TABLE[animal_type][VISIBILITY_C];
	agility = CREATURES_TABLE[animal_type][AGILITY_C];
	strength = CREATURES_TABLE[animal_type][STRENGTH_C];
	food_value = CREATURES_TABLE[animal_type][WEIGHT] / KILOS_PER_FOOD_VALUE;
	water_value = CREATURES_TABLE[animal_type][WEIGHT] / KILOS_PER_WATER_VALUE;
}

void Animal::PlaceMeNear(uint16_t _y, uint16_t _x){
	uint16_t weidth = field->get_width(), height = field->get_height();
  
	uint16_t step = 1;
	for (int i = 1; i < visibility * 2 + 1; i++){
		for (int h = 0; h < i; h++){
			if (_y >= 0 && _y < height && _x >= 0 && _x < weidth){
				Object* obj = field->get(_y, _x);
				if (obj == nullptr){
					x = _x, y = _y;
					field->set(y, x, this);
					return ;
				}
			}
			_x += step;
		}
		for (int h = 0; h < i; h++){
			if (_y >= 0 && _y < height && _x >= 0 && _x < weidth){
				Object* obj = field->get(_y, _x);
				if (obj == nullptr){
					x = _x, y = _y;
					field->set(y, x, this);
					return ;
				}
			}
			_y += step;
		}
  
		step = -step;
	}
	throw std::runtime_error("PlaceMeNear: Cannot find place");
} 

uint16_t* Animal::where() { return new uint16_t[2] {y, x}; };

void Animal::BasicLive(){
	uint16_t* target = CheckForTarget();
	if (!target){
		Ramble();
		return ;
	}
	else if(field->get(target[0], target[0])->obj_type == ANIMAL){
		Animal* animal = (Animal*)field->get(target[0], target[1]);
		Edge edge = Edge(static_cast<uint8_t>(animal->who()), static_cast<uint8_t>(this->animal_type), 0);
		if (CHAINING.find(hash(edge.who_eats, edge.who_is_eaten, edge.is_plant)) != CHAINING.end()){
			GetOut(target[0], target[1]);
		} else{
			if (abs(y - target[0]) + abs(x - target[1]) == 1){
				uint16_t chance = 0.4;
				if (speed > animal->speed){
				 	chance += 0.2;
				}
				if (strength > animal->strength){
				  	chance += 0.2;
				}
				if (agility > animal->agility){
				  	chance += 0.2;
				}
				if (roll(chance)){
					float food = CREATURES_TABLE[animal->who()][WEIGHT];
					float water = field->get(target[0], target[1])->water_value;
					float capacity = CREATURES_TABLE[animal_type][WEIGHT] / 2;
					hunger = hunger + food < capacity ? hunger + food : capacity;
					thirst = thirst + water < capacity ? thirst + water : capacity;
					field->del(target[0], target[1]);
					AdjustUrge();
				}
			} else{
				GoToTarget(target[1], target[0]);
			}
		}
	}
	else{
		if (abs(x - target[0]) + abs(y - target[1]) == 1){
			if (field->get(target[0], target[1])->obj_type == FOOD){
				float food = field->get(target[0], target[1])->food_value;
				float water = field->get(target[0], target[1])->water_value;
				float capacity = CREATURES_TABLE[animal_type][WEIGHT] / 2;
				hunger = hunger + food < capacity ? hunger + food : capacity;
				thirst = thirst + water < capacity ? thirst + water : capacity;
				field->del(target[0], target[1]);
			}
			else{
				float water = CREATURES_TABLE[animal_type][WEIGHT] / 5;
				float capacity = CREATURES_TABLE[animal_type][WEIGHT] / 2;
				thirst = thirst + water < capacity ? thirst + water : capacity;
			}
			AdjustUrge();
		} else {
			GoToTarget(target[1], target[0]);
		}
	}
}

void Male::ApplyMaleGene() {
	attractiveness = ATTRACT_F & male_gene + ATTRACT_S & male_gene;
}


Male::Male(ANIMAL_TYPE _animal_type) {
	animal_type = _animal_type;
	gender_type = MALE;
	ShuffleBasicParameters();
	
	ApplyMaleGene();
}

Male::Male(uint16_t& father_basic_gene, uint16_t& father_male_gene, Female* mother) {
	animal_type = mother->who();
	gender_type = MALE;
	ApplyChildParameters();

	//genes set
	basic_gene = GenerateGene(father_basic_gene, mother->GetBasicGene());
	ApplyBasicGene();
	male_gene = father_male_gene;
	ApplyMaleGene();

	uint16_t* mother_coord = mother->where();
	try{
		PlaceMeNear(mother_coord[0], mother_coord[1]);
	} catch (std::exception& e){
		std:: cout << e.what() << std::endl;
	}
	delete[] mother_coord;
	//Set location(near mother) TODO
}

void Female::Mating(uint16_t father_male_gene, uint16_t father_basic_gene) {
	recieved_male_gene  = father_male_gene;
	recieved_basic_gene = father_basic_gene;
	hired               = false;
	pregnant            = true;
	cur_preg_time       = CREATURES_TABLE[animal_type][PREGNANCY_TIME] + (preg_quality - 1) * CREATURES_TABLE[animal_type][PREGNANCY_GENE]; //base pregnancy time +- base growth pregnancy time
}

void Female::GiveBirth(){
	auto childrenCount = CREATURES_TABLE[animal_type][CHILDREN_COUNT];
	auto pregGene      = CREATURES_TABLE[animal_type][PREGNANCY_GENE];
	auto pregTime      = CREATURES_TABLE[animal_type][PREGNANCY_TIME];

	uint16_t max_cnt_child = childrenCount * (1.f + (preg_quality - 1.f) * pregGene / pregTime);
	uint16_t cnt_child = rand() % (int)CREATURES_TABLE[animal_type][CHILDREN_COUNT] + 1;
	for (int i = 0; i < cnt_child; i++){
		bool gender = rand() % 2;
		if (gender){
			Male* child = new Male(recieved_basic_gene, recieved_male_gene, this);
		} else{
			Female* child = new Female(recieved_basic_gene, this);
		}
	}
}

void Female::ApplyFemaleGene() {
	preg_quality = PREGNANCY_F & female_gene + PREGNANCY_S & female_gene;
}

Female::Female(ANIMAL_TYPE _animal_type) {
	animal_type = _animal_type;
	gender_type = FEMALE;
	ShuffleBasicParameters();

	ApplyFemaleGene();
}
Female::Female(uint16_t& father_basic_gene, Female* mother) {
	animal_type = mother->animal_type;
	gender_type = MALE;
	ApplyChildParameters();

	//genes set
	basic_gene = GenerateGene(father_basic_gene, mother->basic_gene);
	ApplyBasicGene();
	female_gene = mother->female_gene;
	ApplyFemaleGene();

	uint16_t* mother_coord = mother->where();
	try{
		PlaceMeNear(mother_coord[0], mother_coord[1]);
	} catch (std::exception& e){
		std:: cout << e.what() << std::endl;
	}
	delete[] mother_coord;
	//Set location(near mother) TODO

}

void Male:: Live(){
	AdjustAnimalForAge();
	if(++current_age > CREATURES_TABLE[animal_type][LIFESPAN]){
		field->del(y, x);
	}
	if (partner || (repruductive_urge > thirst_urge && repruductive_urge > hunger_urge)){
		uint16_t* target = MCheckForTarget();
		if (!target && !partner){
			Ramble();
			return ;
		}
		Animal* animal = (Animal*)field->get(target[0], target[1]);
		Edge edge = Edge(static_cast<uint8_t>(animal->who()), static_cast<uint8_t>(animal_type), 0);
		if (CHAINING.find(hash(edge.who_eats, edge.who_is_eaten, 0)) != CHAINING.end()){
			if(partner){
				partner->hired = false;
				partner = nullptr;
			}
			GetOut(target[0], target[1]);
		} else if(partner){
			if (partner->hired){
				uint16_t* partner_coord = partner->where();
				if (abs(y - partner_coord[0]) + abs(x - partner_coord[1]) > 1){
					GoToTarget(partner_coord[1], partner_coord[0]);
				} else{
					Mating();
				}
			} else{
				partner = nullptr;
				Ramble();
			}
		} else{
			Female* female = (Female*)animal;
			if (SendMateRequest(female)){
				GoToTarget(target[1], target[0]);
			} else{
				Ramble();
			}
		}

	} else{
		BasicLive();
	}
}

bool Male::SendMateRequest(Female* partner) {
	if (partner->RecieveMateRequest(attractiveness)) { //success
		this->partner = partner;
		return true;
	}
	return false;
}

bool Female::RecieveMateRequest(uint16_t attactiveness) {
	if (roll(AGREE_CHANCE + repruductive_urge / 200 * (1 - AGREE_CHANCE) + (attactiveness - 1) * (1 - AGREE_CHANCE) / 2)) { //ugly = -chance, ok = +0, beautiful = +chance
		hired = true;
		return true;
	}
	return false;
}

void Female:: Live(){
	AdjustAnimalForAge();
	if(++current_age > CREATURES_TABLE[animal_type][LIFESPAN]){
		field->del(y, x);
	}
	if (pregnant && --cur_preg_time == 0){
		GiveBirth();
	} else if (hired){
		uint16_t* target = CheckForTarget();
		if (field->get(target[0], target[1])->obj_type == ANIMAL){
			Animal* animal = (Animal*)field->get(target[0], target[1]);
			Edge edge = Edge(static_cast<uint8_t>(animal->who()), static_cast<uint8_t>(animal_type), 0);
			if (CHAINING.find(hash(edge.who_eats, edge.who_is_eaten, 0)) != CHAINING.end()){
				hired = false;
				GetOut(target[0], target[1]);
			}
		}
		return ;
	} else{
		BasicLive();
	}
}