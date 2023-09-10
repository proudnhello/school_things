#include "Character.h"

#include <iostream>

#include "Room.h"

using namespace std;

void Character::set_current_room(std::shared_ptr<Room> room) {
	current_room = room;
}

std::shared_ptr<Room> Character::get_current_room() {
	return current_room;
}

void Character::look() {

	if (current_room == nullptr) {
		std::cout << "You are nowhere" << endl;
		return;
	}

	cout << current_room->get_description() << endl << endl;

}


