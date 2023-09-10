#pragma once

#include "picojson.h"

// a collection of utilities for verifying JSON fields
// https://cplusplus.com/doc/oldtutorial/templates/

template<typename T> bool has_field(const picojson::value::object & obj, const std::string & fieldname) {

        // illustates data verification - does this object have the named field, and is it of type T?

        if (obj.find(fieldname) == obj.end()) return false;

	std::string fn = fieldname;

        if (!obj.find(fn)->second.is<T>()) return false; // <-- HINT: this is a templated method

        return true;

}


