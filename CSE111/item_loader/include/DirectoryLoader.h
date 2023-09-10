#pragma once

// A Loader for objects it in the system - loads objects from the filesystem
// This loader loads a whole directory of objects, and inserts them into the
// specified index

#include <string>
#include <memory>

class DirectoryLoader {

	public:
		bool load_directory_of_items(const std::string & dirname) const;


};
