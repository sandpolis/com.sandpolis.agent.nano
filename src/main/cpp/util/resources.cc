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

#include "util/resources.hh"
#include <unistd.h>

bool s7s::resource_load(std::string path, Resource &output) {

	char readlink[32];
	std::uintmax_t exe_size;

	// Determine executable path
	sprintf(readlink, "readlink /proc/%d/exe", getpid());
	std::string exe = s7s::exec(readlink);

	try {
		exe_size = fs::file_size(exe);
	} catch (fs::filesystem_error &e) {
		return false;
	}

	int fd = open(exe.c_str(), O_RDONLY);
	if (fd <= 0) {
		return false;
	}

	// Map the executable
	void *data = mmap(nullptr, exe_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (data == nullptr) {
		return false;
	}

	// Move to the end
	void *d = data + exe_size - 4;

	// Walk resources backwards
	while (*static_cast<std::uint32_t*>(d) > 0) {

		// Read path size
		std::uint32_t res_path_size = *static_cast<std::uint32_t*>(d);

		// Give up if there's any hint of corruption
		if (res_path_size > MAX_PATH) {
			munmap(data, exe_size);
			return false;
		}

		// Read path
		d -= res_path_size;
		std::string res_path(static_cast<char*>(d), res_path_size);

		// Read resource length
		d -= 8;
		std::uint64_t res_size = *static_cast<std::uint64_t*>(d);

		d -= res_size;

		if (path == res_path) {
			// Found the requested resource
			output.load(d, res_size);
			munmap(data, exe_size);
			return true;
		}

		d -= 4;
	}
	munmap(data, exe_size);
	return false;
}
