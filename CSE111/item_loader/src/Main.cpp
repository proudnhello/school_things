
#include <iostream>

#include "DirectoryLoader.h"


int main() {

	std::cout << "Loading your items" << std::endl;

	DirectoryLoader loader;
	
	if (loader.load_directory_of_items("./data/items/")) {
		std::cout << "SUCCESS : your items loaded correctly" << std::endl;
		exit(EXIT_SUCCESS);
	}

	std::cout << "FAILURE : one or more items failed to load " << std::endl;
}

