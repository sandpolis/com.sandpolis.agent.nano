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

#include <string>
#include <sys/mman.h>
#include <iostream>
#include <fcntl.h>

template <typename T>
T *resource_load(std::string path) {

	char readlink[32];
	sprintf(readlink, "readlink /proc/%d/exe", getpid());
	if (system(readlink)) {
		
	}

	int fd = open("/home/cilki/img.six", O_RDONLY);
	if (fd <= 0) {
		std::cout << "Failed to open device" << std::endl;
		return nullptr;
	}

	std::size_t file_size = 25682;
	if (file_size <= 0) {
		std::cout << "Failed to get resource size" << std::endl;
		return nullptr;
	}

	void *file = mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file == nullptr) {
		std::cout << "Failed to map resource" << std::endl;
		return nullptr;
	}

	// Walk resources backwards
	for (unsigned int i = file_size - 1;;) {
		
	}
	return nullptr;
}