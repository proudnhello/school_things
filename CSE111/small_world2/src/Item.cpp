#include "Item.h"

#include <picojson.h>

bool Item::from_json(const std::string & filename, picojson::object & json_obj) {


	// this is where you will implement the loader for your Item
	// obviously, the item will have the common properties in GameObject, so we do that
	// super class method first
	
	return GameObject::from_json(filename, json_obj);

	// but then, you will need to load the additional properties from json
	// for the instance variables (properties) that you have defined in Item.h 
	// as protected member variables
	

}

