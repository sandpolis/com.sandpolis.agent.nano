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
#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <iostream>
#include <sys/mman.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <chrono>
#include <fcntl.h>
#include <string>

// Restore a block-mode snapshot
bool snapshot_block_write();

// Create a new block-mode snapshot
bool snapshot_block_read(std::string device);//, size_t block_size, unsigned long &bytes_read);

// Restore a file-mode snapshot
bool snapshot_file_write();

// Create a new file-mode snapshot
bool snapshot_file_read();

#endif
