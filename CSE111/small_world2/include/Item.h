#pragma once

#include <string>

#include "GameObject.h"

class Item : public GameObject {

	protected:

		// Add properties for your object here.  You will need to stick to what picojson supports
		// I.E. bool, std::string, numbers : https://www.w3schools.com/js/js_json_datatypes.asp
		// For JSON numbers, double is the right C++ type to use
		//
		// Then you will need to add a corresponding json object with the properties in that
		// as a JSON map
		//
		// example: double active_time;     // defines how long this item remains "active" after it has been "activated"

	public:
		Item() : GameObject() {
			// This is the default contructor, so you will want to set sensible defaults
			// for your properties in here		
		}

		// You don't need to define a constructor with loads of args - that's loaded from JSON

		virtual bool from_json(const std::string & filename, picojson::object & json_obj) override;

		virtual bool is_item() const override { return true; }
		virtual bool can_be_taken() const override { return true; }

};
