#pragma once

#include <string> 
#include <map>
#include <memory>

#include "Player.h"

class Room : public GameObject {

	protected:

		// all of the items in this room
		std::map<std::string, std::shared_ptr<GameObject>> contents;

		// a map which links a symbolic direction, like "north" to the id of another room
		// when the player types "go north" we will use this to lookup what room to transit to
		std::map<std::string, std::string> links_to_other_rooms;

		bool load_links(const std::string & filename, picojson::object & link_obj); 

	public:
		Room() : GameObject() {}
		Room(const std::string & id_, const std::string name_, const std::string & desc_) : GameObject(id_, name_, desc_) {}

		virtual bool from_json(const std::string & filename, picojson::object & obj) override;

		virtual bool is_room() const { return true; }

		virtual bool add_contents(std::shared_ptr<GameObject> object) {
			if (!object->is_character() && !object->is_item()) return false;
			contents[object->get_id()] = object;
			return true;
		}

		virtual void remove_contents(std::shared_ptr<GameObject> object) {
			contents.erase(object->get_id());
		}

		virtual void add_link(const std::string & direction, const std::string & to_id) {
			links_to_other_rooms[direction] = to_id;
		}

		virtual std::string get_link(const std::string & direction);
		virtual void clear(); 
		virtual std::string get_next_room_id(const std::string & direction);

};
