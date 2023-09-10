#include "Item.h"

#include <picojson.h>
#include "json_utils.h"

bool Item::from_json(const std::string & filename, picojson::object & json_obj) {


	// this is where you will implement the loader for your Item
	// obviously, the item will have the common properties in GameObject, so we do that
	// super class method first
	
	if(!GameObject::from_json(filename, json_obj)){
		return false;
	}

	// but then, you will need to load the additional properties from json
	// for the instance variables (properties) that you have defined in Item.h 
	// as protected member variables

	if (!has_field<bool>(json_obj, "Takeable")) {
		std::cerr << "filename : " << filename << " does not contain an Takeable property (or it is not a string)" << std::endl;
		return false;
	}
	if (!has_field<bool>(json_obj, "Moveable")) {
		std::cerr << "filename : " << filename << " does not contain an Moveable property (or it is not a string)" << std::endl;
		return false;
	}

	takeable = json_obj["Takeable"].get<bool>();
    moveable = json_obj["Moveable"].get<bool>();

	/* Testing print statements
	std::cout << get_id() << "\n";
	std::cout << get_name() << "\n";
	std::cout << get_description() << "\n";
	std::cout << is_item() << "\n";
	std::cout << can_be_taken() << "\n";
	std::cout << can_be_moved() << "\n";
	*/

	return true;

}

