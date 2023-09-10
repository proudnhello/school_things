
#include "Loader.h"

#include <fstream>

#include "picojson.h"
#include "json_utils.h"

#include "Maker.h"

#include "Item.h"
#include "UseableItem.h"
#include "Equipable.h"
#include "Weapon.h"


// loads items of various types from the file system

using namespace picojson;

Loader::Loader() {
	// this makes only one thing - Items
	makers["Item"] = new SpecificMaker<Item>();
	makers["UseableItem"] = new SpecificMaker<UseableItem>();
	makers["Equipable"] = new SpecificMaker<Equipable>();
	makers["Weapon"] = new SpecificMaker<Weapon>();
}

Loader::~Loader() {
	for (auto mkr : makers) { delete mkr.second; }  // DELETE THINGS WE NEWED
}

Maker * Loader::get_maker(const std::string & type) const {

	// gets a maker for the specific type of object

	std::map<std::string, Maker*>::const_iterator it = makers.find(type);
	
	if (it == makers.end()) {
		std::cout << "Could not find maker for objects of type : " << type << " - object construction failed" << std::endl;
		return nullptr;
	}

	return it->second;
}


std::shared_ptr<GameObject> Loader::load_obj(const std::string & filename) const {

	// load a room from the JSON contained in filename
	// if there are errors, note these to std::cerr, and return nullptr
	// otherwise, return a shared_ptr to the room

	picojson::value v;
	std::ifstream file(filename);

	file >> v;

	std::string err = picojson::get_last_error();

	if (!err.empty()) {
		std::cerr << "Error reading file : " << filename << " : " << err << std::endl;
		return nullptr;
	}

	if (!v.is<picojson::object>()) {
		std::cerr << "Contents of file : " << filename << " are not a valid JSON object : " << v.to_str() << std::endl;
		return nullptr;
	}
	
	// it's an object, so uses it as such	
	picojson::object& obj = v.get<picojson::object>();

	if (!has_field<std::string>(obj, "Type")) {
		std::cerr << "filename: " << filename << " does not contain a Type property (or is is not a string)" << std::endl;
		return nullptr;
	}

	// do we have a maker for this type (Room, Item, etc??)

	picojson::value vType = obj["Type"];
	Maker * maker = get_maker(vType.get<std::string>());

	if (maker == nullptr) {
		std::cerr << "filename: " << filename << " defines an object type : " << vType.get<std::string>() << " but no maker exists for it" << std::endl;
		return nullptr;
	}

	// deletgate the work of actually making the object to the maker

	return maker->make(filename, obj);

}





