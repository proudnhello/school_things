
#include <iostream>

#include "Player.h"
#include "Room.h"
#include "Index.h"
#include "DirectoryLoader.h"

#include "trim.h"
#include "split.h"

std::shared_ptr<Room> get_next_room(Index<Room> * index, std::shared_ptr<Room> room, const std::string & direction) {

        const std::string & next_room_id = room->get_next_room_id(direction);

        if (next_room_id == "") {
                std::cout << "There is no where to go in the " << direction << " direction" << std::endl;
                return nullptr;
        }

        std::shared_ptr<Room> proposed_room = index->get_object(next_room_id);

        if (proposed_room == nullptr) {
                std::cout << "There was a room proposed for " <<  direction << " but it does not exist in the index" << std::endl;
                return nullptr;
        }

        return proposed_room;
}

int main() {

	std::cout << "Creating the player" << std::endl;
	Player player("Player1", "Player1", "A non-descript player.  They are grey-ish"); 
	Index<Room> rooms;

	std::cout << "Loading the rooms" << std::endl;
	DirectoryLoader loader;
	loader.load_directory_of_rooms("./data/rooms/", &rooms);
	
	std::string starting_room = "mrober10-room-a";
	std::shared_ptr<Room> room = rooms.get_object(starting_room);

	std::cout << "Getting the starting room " << std::endl;

	if (room == nullptr) {
		std::cerr << "Cannot find the starting room : " << starting_room << std::endl;
		rooms.clear();
		player.clear();
		return EXIT_FAILURE;
	}

	player.set_current_room(room);
	std::cout << std::endl << std::endl;

	player.look();

	std::string input_line;
	
	do {

		std::getline(std::cin, input_line);
	 	input_line = trim(input_line);		

		if ((input_line == "q") || (input_line == "Q") || (input_line == "quit")) {
			std::cout << "bye" << std::endl;
			rooms.clear();
			player.clear();
			return EXIT_SUCCESS;
		}

		if (input_line == "look") {
			player.look();
			continue;
		}

		if (input_line.rfind("go", 0) == 0) {

			std::vector<std::string> sp = split(input_line, " ");	

			if (sp.size() == 1) {
				std::cout << "Go where?" << std::endl;
				continue;
			}

			const std::string direction = sp[1];

			std::cout << " go " << direction << std::endl;

			std::shared_ptr<Room> current_room = player.get_current_room();

			if (current_room == nullptr) {
				std::cout << "You are standing nowhere, so can't go anywhere" << std::endl;
				continue;
			}

			std::shared_ptr<Room> proposed_room = get_next_room(&rooms, current_room, direction);
			if (proposed_room == nullptr) {
				std::cout << "There isn't anything in that direction" << std::endl;
				continue;
			}
				
			player.set_current_room(proposed_room);
			std::cout << std::endl << std::endl;

			player.look();

			continue;

		}

		std::cout << "I'm sorry, I don't understand that" << std::endl;

	} while (true);


}

