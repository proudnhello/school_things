#pragma once

#include <string>

#include "Item.h"

class UseableItem : public Item {

	protected:

		std::string affectedItems;
		std::string affectedMethod;
		// Add properties for your object here.  You will need to stick to what picojson supports
		// I.E. bool, std::string, numbers : https://www.w3schools.com/js/js_json_datatypes.asp
		// For JSON numbers, double is the right C++ type to use
		//
		// Then you will need to add a corresponding json object with the properties in that
		// as a JSON map
		//
		// example: double active_time;     // defines how long this item remains "active" after it has been "activated"

	public:
		UseableItem() : Item() {
			// This is the default contructor, so you will want to set sensible defaults
			// for your properties in here		
            affectedItems = "";
            affectedMethod = "";
		}

		// You don't need to define a constructor with loads of args - that's loaded from JSON

		virtual bool from_json(const std::string & filename, picojson::object & json_obj) override;

		virtual bool is_useable() const { return true; }
        virtual std::string affects_items() const { return affectedItems; }
        virtual std::string affects_methods() const { return affectedMethod; }

};
