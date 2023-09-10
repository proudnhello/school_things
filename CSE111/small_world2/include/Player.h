#pragma once

#include "Character.h"

class Player : public Character {

	public:
		Player() : Character() {}
		Player(const std::string & id_, const std::string name_, const std::string & desc_) : Character(id_, name_, desc_) {}


};
