#pragma once

#include <string>
#include <memory>

// look up objects by their ID.  This is a templated class, we specify the type - in this 
// case, a Room, Player, or Item

template <typename T> class Index {

	std::map<std::string, std::shared_ptr<T>> index;

	public:

		std::shared_ptr<T> get_object(const std::string & id) {
			auto it = index.find(id);
			if (it == index.end()) return nullptr;
			return it->second;
		}

		void add_object(std::shared_ptr<T> object) {
			index[object->get_id()] = object;
		}

		void clear() {
			index.clear();
		}

};
