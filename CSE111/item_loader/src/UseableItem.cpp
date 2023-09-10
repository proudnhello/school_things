#include "UseableItem.h"

#include <picojson.h>
#include "json_utils.h"

bool UseableItem::from_json(const std::string & filename, picojson::object & json_obj) {


	// this is where you will implement the loader for your Item
	// obviously, the item will have the common properties in GameObject, so we do that
	// super class method first
	
	if(!Item::from_json(filename, json_obj)){
		return false;
	}

	// but then, you will need to load the additional properties from json
	// for the instance variables (properties) that you have defined in Item.h 
	// as protected member variables

	if (!has_field<std::string>(json_obj, "AffectedItems")) {
		std::cerr << "filename : " << filename << " does not contain an Takeable property (or it is not a string)" << std::endl;
		return false;
	}
	if (!has_field<std::string>(json_obj, "AffectedMethod")) {
		std::cerr << "filename : " << filename << " does not contain an Moveable property (or it is not a string)" << std::endl;
		return false;
	}

	affectedItems = json_obj["AffectedItems"].get<std::string>();
    affectedMethod = json_obj["AffectedMethod"].get<std::string>();

	//Testing print statements
    /*
	std::cout << get_id() << "\n";
	std::cout << get_name() << "\n";
	std::cout << get_description() << "\n";
	std::cout << is_item() << "\n";
	std::cout << can_be_taken() << "\n";
	std::cout << can_be_moved() << "\n";
    std::cout << is_useable() << "\n";
	std::cout << affects_items() << "\n";
	std::cout << affects_methods() << "\n";
    */
	return true;

}