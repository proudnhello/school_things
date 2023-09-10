#pragma once

#include "GameObject.h"

class Room;

class Character : public GameObject {

	protected:

		std::shared_ptr<Room> current_room = nullptr;

	public:
		Character() : GameObject() {}

		Character(const std::string & id_, const std::string name_, const std::string & desc_) : GameObject(id_, name_, desc_) {}

		virtual bool is_character() const { return true; }

		virtual void set_current_room(std::shared_ptr<Room> room);
		virtual std::shared_ptr<Room> get_current_room();

		virtual void look();
		virtual void clear() { current_room = nullptr; }

};
