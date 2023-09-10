#pragma once

// A Loader for objects it in the system - loads objects from the filesystem

#include <string>
#include <memory>

#include "picojson.h"
#include "Maker.h"

using namespace picojson;


class Loader {

	protected:

		std::map<std::string, Maker*> makers;
		Maker * get_maker(const std::string & type) const;

	public:
		Loader();
		~Loader();

		std::shared_ptr<GameObject> load_obj(const std::string & filename) const;


};
