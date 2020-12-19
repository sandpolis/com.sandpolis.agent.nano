//============================================================================//
//                                                                            //
//                Copyright © 2015 - 2020 Subterranean Security               //
//                                                                            //
//  This source file is subject to the terms of the Mozilla Public License    //
//  version 2. You may not use this file except in compliance with the MPL    //
//  as published by the Mozilla Foundation at:                                //
//                                                                            //
//    https://mozilla.org/MPL/2.0                                             //
//                                                                            //
//=========================================================S A N D P O L I S==//
#ifndef RESOURCES_H
#define RESOURCES_H

#include "util/exec.hh"
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <sys/mman.h>
#include <utility>

namespace fs = std::filesystem;

template <typename T>
T resource_load(std::string path) {

	char readlink[32];
	std::uintmax_t exe_size;

	// Determine executable path
	sprintf(readlink, "readlink /proc/%d/exe", getpid());
	std::string exe = s7s::exec(readlink);

	try {
        exe_size = fs::file_size(exe);
    } catch(fs::filesystem_error& e) {
        return nullptr;
    }

	int fd = open(exe.c_str(), O_RDONLY);
	if (fd <= 0) {
		return nullptr;
	}

	// Map the executable
	void *data = mmap(nullptr, exe_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (data == nullptr) {
		return nullptr;
	}

	// Walk resources backwards
	for (unsigned int i = file_size - 1;;) {
		std::uint32_t i_index = *static_cast<std::uint32_t *>(d);
		std::uint64_t i_file_size = *static_cast<std::uint64_t *>(d - 4);
		std::uint64_t i_path_size = *static_cast<std::uint64_t *>(d - 12);

		// Check path
		std::string i_path(static_cast<char *>(d), i_path_size);
		if (path == i_path) {
			// Found the requested resource
			return T(d, i_file_size);
		}
	}
	return nullptr;
}

// A resource without known structure
class RawResource {

	// The read-only data
	void *data;

public:

	// The data length in bytes
	const std::size_t length;

	RawResource(void *_data, std::size_t _length) : length(_length) {
		data = malloc(_length);
		memcpy(data, _data, _length);
	}

	~RawResource() {
		free(data);
	}

	void *get_data() {
		return data;
	}
};

// A resource backed by 
class PropertiesResource {

	std::map<std::string, std::string> properties;

public:

	PropertiesResource(void *data, std::size_t length) {

		// Manage a pointer to the start of the current element and to the
		// current character
		char *d = static_cast<char *>(data);
		char *e = static_cast<char *>(data);
		for (int c = 0; c < length; c++) {
			if (e[c] == '=') {
				std::string key(d, c - (d - e));
				d = e + c + 1;
				for (; c < length; c++) {
					if (e[c] == '\n') {
						std::string value(d, c - (d - e));
						d = e + c + 1;
						properties.insert(std::make_pair(key, value));
					}
				}
			}
		}
	}

	std::string get_property(std::string name) {
		return properties[name];
	}
};

#endif
