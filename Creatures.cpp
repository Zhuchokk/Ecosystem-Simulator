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