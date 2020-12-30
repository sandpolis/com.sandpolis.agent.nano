//============================================================================//
//                                                                            //
//                         Copyright © 2015 Sandpolis                         //
//                                                                            //
//  This source file is subject to the terms of the Mozilla Public License    //
//  version 2. You may not use this file except in compliance with the MPL    //
//  as published by the Mozilla Foundation.                                   //
//                                                                            //
//============================================================================//
#include "snapshot.hh"
#include "util/murmur.hh"
#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

bool snapshot_block_read(std::string device) { //, size_t block_size, unsigned long &bytes_read) {
	int fd = open("", O_RDONLY);
	if (fd <= 0) {
		std::cout << "Failed to open device" << std::endl;
		return false;
	}

	size_t device_size = 536870912;
	if (device_size <= 0) {
		std::cout << "Failed to get device size" << std::endl;
		return false;
	}

	void *blocks = mmap(nullptr, device_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (blocks == nullptr) {
		std::cout << "Failed to map device" << std::endl;
		return false;
	}

	void *block;
	char hash[16];
	size_t block_size = 4096;
	for (unsigned long i = 0; i < device_size; i += block_size) {
		block = blocks + i;

		// Hash the block
		MurmurHash3_128(block, block_size, 0, &hash);
		//bytes_read += block_size;

		// TODO: egress
	}

	return true;
}

bool snapshot_block_write(char *device, size_t block_size,
		unsigned long &bytes_written) {
	int fd = open(device, O_WRONLY);
	if (fd <= 0) {
		std::cout << "Failed to open device" << std::endl;
		return false;
	}

	return true;
}
