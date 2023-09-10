#pragma once

#include "picojson.h"
#include "GameObject.h"

// maker makes objects of type T using JSON input data
// if it cannot make the object, it returns null

class Maker {

	// this is our generic baseclass for Makers - it provides the make() signature

	public:

		virtual std::shared_ptr<GameObject> make(const std::string & filename, picojson::object & obj) = 0;

};

template<typename T> class SpecificMaker  : public Maker {

	// this is a specific maker - it makes instances of type T and returns
	// them as a generic shared_ptr on T

	public:
		SpecificMaker() {}

		virtual std::shared_ptr<GameObject> make(const std::string & filename, picojson::object & json_obj) {
			
			// we make "blank"  instance of Type T (Room, Item, etc)
			std::shared_ptr<T> obj = std::make_shared<T>();

			// and then we delegate the loading from JSON to it
			if (!obj->from_json(filename, json_obj)) return nullptr;

			return obj;

		}

};
