#include<iostream>
#include"Creatures.h"
#include<list>
#include<thread>

//Static vars defenition
int Compare::x = 0;
int Compare::y = 0;
Field* Animal::field = NULL;

int main()
{
	//just plug
	char a; std::cout << "Do you want to start(y/n)?"; std::cin >> a;
	if (a != 'y') return 0;
	//========
	//TODO: place food
	Field field = Field(100, 100, 40, 20);
	Animal::field = &field; // init static field for all animals
	std::list<Object*> creatures;
	for (int i = 0; i < INIT_COUNT_OF_CREATURES; ++i) {
		ANIMAL_TYPE type = (ANIMAL_TYPE)(rand() % COUNT_ANIMAL_TYPES);
		bool gender = rand() % 2;
		if (gender) {
			Male* m = new Male(type);
			creatures.push_back(m);
		}
		else {
			Female* f = new Female(type);
			creatures.push_back(f);
		}
			
	}

	//For now there are only animals in creatures, since there is nothing to do with food and water
	while (true) {
		for (auto iter = creatures.begin(); iter != creatures.end(); iter++) {
			if (*iter == nullptr) {
				iter = creatures.erase(iter); //delete not existing objects and get new valid iterator
			}

			if ((*iter)->obj_type == ANIMAL) {
				Animal* ref = (Animal*)(*iter);
				if (ref->get_gender() == MALE) {
					Male* ref2 = (Male*)ref;
					ref2->Live();
				}
				else if (ref->get_gender() == FEMALE) {
					Female* ref2 = (Female*)ref;
					ref2->Live();
				}
			}
			else if ((*iter)->obj_type == FOOD) {
				//do smth with food
			}
			else if ((*iter)->obj_type == WATER) {
				//do smth with water
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS));
	}
	

	return 0;
}
