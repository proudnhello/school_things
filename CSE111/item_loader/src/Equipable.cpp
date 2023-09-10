#include "Equipable.h"

#include <picojson.h>
#include "json_utils.h"

bool Equipable::from_json(const std::string & filename, picojson::object & json_obj) {


	// this is where you will implement the loader for your Item
	// obviously, the item will have the common properties in GameObject, so we do that
	// super class method first
	
	if(!UseableItem::from_json(filename, json_obj)){
		return false;
	}

	// but then, you will need to load the additional properties from json
	// for the instance variables (properties) that you have defined in Item.h 
	// as protected member variables

	if (!has_field<double>(json_obj, "StatsEffect")) {
		std::cerr << "filename : " << filename << " does not contain an Takeable property (or it is not a string)" << std::endl;
		return false;
	}
	if (!has_field<std::string>(json_obj, "Slot")) {
		std::cerr << "filename : " << filename << " does not contain an Moveable property (or it is not a string)" << std::endl;
		return false;
	}

	statsEffect = json_obj["StatsEffect"].get<double>();
    slot = json_obj["Slot"].get<std::string>();

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
    std::cout << get_stats() << "\n";
	std::cout << get_slot() << "\n";
    */
	return true;

}