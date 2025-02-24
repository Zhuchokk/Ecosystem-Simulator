#include<iostream>
#include"Creatures.h"
#include<list>
#include<thread>

int main()
{
	//just plug
	char a; std::cout << "Do you want to start(y/n)?"; std::cin >> a;
	if (a != 'y') return 0;
	//========
	//TODO: init field, place objects and creatures
	std::list<Object*> creatures;
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
