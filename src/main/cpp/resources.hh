//============================================================================//
//                                                                            //
//                         Copyright © 2015 Sandpolis                         //
//                                                                            //
//  This source file is subject to the terms of the Mozilla Public License    //
//  version 2. You may not use this file except in compliance with the MPL    //
//  as published by the Mozilla Foundation.                                   //
//                                                                            //
//============================================================================//
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

namespace s7s::resources {

	// The longest valid resource path
	#define MAX_PATH 64

	struct Resource {
		virtual bool load(void *data, std::size_t length) = 0;
	};

	// A resource without known structure
	class RawResource: public Resource {

		// The resource data
		void *_data;

		// The data length in bytes
		std::size_t _length;

		bool loaded = false;

	public:

		bool load(void *data, std::size_t length) {
			if (loaded) {
				return false;
			}
			_length = length;
			_data = malloc(length);
			if (_data == nullptr) {
				return false;
			}
			loaded = true;
			memcpy(_data, data, length);
			return true;
		}

		~RawResource() {
			if (loaded) {
				free(_data);
			}
		}

		void* data() {
			return _data;
		}

		std::size_t length() {
			return _length;
		}
	};

	// A resource containing a Java properties file
	class PropertiesResource: public Resource {

		std::map<std::string, std::string> properties;

	public:

		bool load(void *data, std::size_t length) {

			// Manage a pointer to the start of the current element and to the
			// current character
			char *d = static_cast<char*>(data);
			char *e = static_cast<char*>(data);
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

			return true;
		}

		std::string get_property(std::string name) {
			return properties[name];
		}
	};

	bool resource_load(std::string path, Resource &output);

}
#endif
