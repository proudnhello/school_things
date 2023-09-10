// A Loader for objects it in the system - loads objects from the filesystem
// This loader loads a whole directory of objects, and inserts them into the
// specified index

#include "DirectoryLoader.h"

#include <string>
#include <memory>

#include <iostream>

#include "listdir.h"
#include "string_utils.h"

#include "GameObject.h"
#include "Item.h"

#include "Loader.h"

bool DirectoryLoader::load_directory_of_items(const std::string & dirname) const {

	Loader loader;
 
	std::cout << "About to directory : " << dirname << std::endl;

	std::vector<std::string> fileNames = list_dir(dirname);
	
	for (const auto & entry : fileNames) {

		if (entry == "..") continue;
		if (entry == ".") continue;

		if (!ends_with(entry, ".json")) continue;	
		
		std::string path = dirname + entry;
		std::cout << "loading: " << path << std::endl;

		std::shared_ptr<GameObject> itemObj = loader.load_obj(path);
		
		if (itemObj == nullptr) {
			std::cerr << "Cannot load item : " << entry << std::endl;
			return false;
		}

		if (!itemObj->is_item()) {
			std::cerr << "Loaded an object for : " << path << " but it isn't a Item!" << std::endl;
		       	return false;	
		}

		// now we have a shared_ptr to a game object.  However, the items are shared_ptrs of Rooms
		// use dynamic_cast to convert the pointer
		//
		
		std::shared_ptr<Item> item = dynamic_pointer_cast<Item>(itemObj);

		if (item == nullptr) {
			std::cerr << "Cannot downcast the itemObj to a item " << std::endl;
			return false;
		}

		std::cout << "Successfully loaded item : " << entry << std::endl;
	}

	return true;

}

