#pragma once

#include <string>
#include <memory>

#include <picojson.h>

// A definition for the "root class" of objects in the system
// We have 3 properties; an id (unique amongst all objects we will load), a name
// (things can share the same name, but would have different IDs) and a description
// which is what we would see if we "look" at the object

// The root class would normally store all "properties" or "attributes" that
// are shared by ALL of the classes which inherit from it
//
// subclasses, like Player, Room, and Item add extra properties to this
// base definition

class GameObject {

	protected:

		std::string id;
		std::string name;
		std::string description;

	public:

		GameObject() : id(""), name(""), description("") {}

		GameObject(const std::string & id_, const std::string & name_, const std::string & description_) {
			id = id_;
			name = name_;
			description = description_;
		}

		virtual bool from_json(const std::string & filename, picojson::object & obj); 

		const std::string & get_id() const { return id;}
		const std::string & get_name() const { return name; }
		const std::string & get_description() const { return description; }

		virtual bool is_player() const { return false; }
		virtual bool is_character() const { return false; }
		virtual bool is_item() const { return false; }
		virtual bool is_room() const { return false; }

		virtual bool can_be_taken() const { return false; }

};
