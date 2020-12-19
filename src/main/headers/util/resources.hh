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

// This file contains randomized placeholder resources that must be substituted
// before runtime.

inline int resource_length(unsigned char *resource) {
	return *reinterpret_cast<int*>(resource);
}

inline unsigned char* resource_body(unsigned char *resource) {
	return resource + 4;
}

template <typename T>
T *resource_load(std::string);

class Resource {

	// The read-only data
	void *data;

	// The data length in bytes
	const std::size_t length;

public:
	Resource(void *_data, std::size_t _length) : length(_length) {
		data = malloc(_length);
		memcpy(data, _data, _length);
	}

	~Resource() {
		free(data);
	}
};

class PropertiesResource : public Resource {
	using Resource::Resource;

public:
	std::string get_property(std::string name) {
		// TODO
		return "";
	}
};

#endif
