// A Loader for objects it in the system - loads objects from the filesystem
// This loader loads a whole directory of objects, and inserts them into the
// specified index

#include "DirectoryLoader.h"
#include "Index.h"

#include <string>
#include <memory>

#include <iostream>

#include "listdir.h"
#include "string_utils.h"

#include "GameObject.h"
#include "Room.h"
#include "Loader.h"

bool DirectoryLoader::load_directory_of_rooms(const std::string & dirname, Index<Room> * roomIndex) const {

	Loader loader;
 
	std::cout << "About to directory : " << dirname << std::endl;

	std::vector<std::string> fileNames = list_dir(dirname);
	
	for (const auto & entry : fileNames) {

		if (entry == "..") continue;
		if (entry == ".") continue;

		if (!ends_with(entry, ".json")) continue;	
		
		std::string path = dirname + entry;
		std::cout << "loading: " << path << std::endl;

		std::shared_ptr<GameObject> roomObj = loader.load_obj(path);
		
		if (roomObj == nullptr) {
			std::cerr << "Cannot load room : " << entry << std::endl;
			return false;
		}

		if (!roomObj->is_room()) {
			std::cerr << "Loaded an object for : " << path << " but it isn't a room!" << std::endl;
		       	return false;	
		}

		// now we have a shared_ptr to a game object.  However, the rooms are shared_ptrs of Rooms
		// use dynamic_cast to convert the pointer
		//
		
		std::shared_ptr<Room> room = dynamic_pointer_cast<Room>(roomObj);

		if (room == nullptr) {
			std::cerr << "Cannot downcast the roomObj to a room (should never happen)" << std::endl;
			return false;
		}

		std::cout << "Successfully loaded room : " << entry << std::endl;

		roomIndex->add_object(room);
	}

	return true;

}

