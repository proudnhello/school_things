#pragma once

#include <string>
#include <vector>

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

static std::vector<std::string> list_dir(const std::string & path) {
	  
	// illustates using C things from inside a C++ program

	struct dirent *entry;
	DIR *dir = opendir(path.c_str());
   
	std::vector<std::string> dirs;

	if (dir == NULL) {
        	return dirs;
    	}

	while ((entry = readdir(dir)) != NULL) {
        	dirs.push_back(entry->d_name);
    	}

    	closedir(dir);

	return dirs;
};
