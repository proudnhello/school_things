#include "Room.h"
#include "Index.h"

std::string Room::get_link(const std::string & direction) {

	// "" is the "error value"

        auto it = links_to_other_rooms.find(direction);
        if (it == links_to_other_rooms.end()) return "";

        return it->second;

}

bool Room::from_json(const std::string & filename, picojson::object & obj) {

	// load the general stuff (from GameObject)
	bool success = GameObject::from_json(filename, obj);
	if (!success) return false; // we failed at the game object level
	
	// load the specific stuff for Rooms
	return load_links(filename, obj["Links"].get<picojson::object>());
}

bool Room::load_links(const std::string & filename, picojson::value::object & obj) {

        // load links to other rooms.  An example of using an iterator to iterate a JSON OBJECT

        for (picojson::value::object::const_iterator i = obj.begin(); i != obj.end(); ++i) {
                const std::string & direction = i->first;
                picojson::value linked_room = i->second;
                if (!linked_room.is<std::string>()) {
                        std::cerr << "Link in filename : " << filename << " named " << direction << " is not a string" << std::endl;
                        return false;
                }

                add_link(direction, linked_room.get<std::string>());
        }

        return true;
}

void Room::clear() {
        contents.clear();
        links_to_other_rooms.clear();
}

std::string Room::get_next_room_id(const std::string & direction) {

        std::map<std::string, std::string>::iterator it = links_to_other_rooms.find(direction);
        if (it == links_to_other_rooms.end()) return "";

        return it->second;
}


