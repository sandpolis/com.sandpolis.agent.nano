//============================================================================//
//                                                                            //
//                         Copyright © 2015 Sandpolis                         //
//                                                                            //
//  This source file is subject to the terms of the Mozilla Public License    //
//  version 2. You may not use this file except in compliance with the MPL    //
//  as published by the Mozilla Foundation.                                   //
//                                                                            //
//============================================================================//
#include "boot/snapshot.hh"
#include "boot/ui.hh"
#include "util/resources.hh"
#include "util/text.hh"
#include <iostream>
#include <locale.h>
#include <sys/mman.h>

// The header window containing the sixel graphic
WINDOW *win_header;

// The stats window
WINDOW *win_stats;

// The surrounding blocks broken into four regions to avoid overlapping windows
WINDOW *win_blocks_north;
WINDOW *win_blocks_west;
WINDOW *win_blocks_east;
WINDOW *win_blocks_south;

// The coordinates of the central dialog box
int DIALOG_TOPLEFT_Y;
int DIALOG_TOPLEFT_X;
int DIALOG_TOPRIGHT_Y;
int DIALOG_TOPRIGHT_X;
int DIALOG_BOTTOMLEFT_Y;
int DIALOG_BOTTOMLEFT_X;
int DIALOG_BOTTOMRIGHT_Y;
int DIALOG_BOTTOMRIGHT_X;

// The maximum number of blocks the window can show
std::size_t MAX_BLOCKS;

// The actual number of blocks to show
std::size_t BLOCK_COUNT;

// The number of UI blocks that correspond with one snapshot block
unsigned int BLOCK_FACTOR = 1;

std::vector<Block> blocks;

// Initialize the header window
bool init_header() {

	if (win_header == nullptr) {
		win_header = newwin(WIN_HEADER_HEIGHT, WIN_HEADER_WIDTH,
				DIALOG_TOPLEFT_Y, DIALOG_TOPLEFT_X);
	}

	// Load sixel image
	s7s::RawResource resource;
	if (!s7s::resource_load("sandpolis.sixel", resource)) {
		return false;
	}

	wmove(win_header, 0, 0);
	wrefresh(win_header);

	// Write image
	puts((static_cast<char*>(resource.data())));
	wmove(win_header, 0, 0);

	return true;
}

// Initialize the stats window and static content
void init_stats() {

	if (win_stats == nullptr) {
		win_stats = newwin(WIN_STATS_HEIGHT, WIN_STATS_WIDTH,
				DIALOG_TOPLEFT_Y + WIN_HEADER_HEIGHT + 1, DIALOG_TOPLEFT_X);
	}

	mvwaddwstr(win_stats, 0, 0,
			L"Creating snapshot of device: /dev/sda1. Please do not interrupt this process.");

	mvwaddwstr(win_stats, 2, 0, L"Time remaining");
	mvwaddwstr(win_stats, 3, 0, L"Network upload");
	mvwaddwstr(win_stats, 4, 0, L"Network download");
	mvwaddwstr(win_stats, 5, 0, L"Disk Read");
	mvwaddwstr(win_stats, 6, 0, L"Disk Write");

	wrefresh(win_stats);
}

void init_blocks() {

	if (win_blocks_north == nullptr) {
		win_blocks_north = newwin(DIALOG_TOPLEFT_Y, COLS, 0, 0);
	}
	if (win_blocks_west == nullptr) {
		win_blocks_west = newwin(DIALOG_HEIGHT, DIALOG_TOPLEFT_X - 1,
				DIALOG_TOPLEFT_Y, 0);
	}
	if (win_blocks_east == nullptr) {
		win_blocks_east = newwin(DIALOG_HEIGHT, COLS - DIALOG_TOPRIGHT_X + 1,
				DIALOG_TOPRIGHT_Y, DIALOG_TOPRIGHT_X + 1);
	}
	if (win_blocks_south == nullptr) {
		win_blocks_south = newwin(LINES - DIALOG_BOTTOMLEFT_Y, COLS,
				DIALOG_BOTTOMLEFT_Y - 1, 0);
	}

	// Prepare an offset which will be used to create a rectangular gap
	int offset = 0;

	// Allocate blocks
	for (int i = 0; i < BLOCK_COUNT; i++) {

		WINDOW *window;

		// Convert block index to absolute coordinates
		int y = (i + offset) / COLS;
		int x = (i + offset) % COLS;

		// If we're about to enter the dialog, add offset and retry this iteration
		if (y >= DIALOG_TOPLEFT_Y && y <= DIALOG_BOTTOMLEFT_Y
				&& x == DIALOG_TOPLEFT_X) {
			offset += DIALOG_WIDTH;
			i--;
			continue;
		}

		// Convert absolute coordinates to relative
		if (y < DIALOG_TOPLEFT_Y) {
			window = win_blocks_north;
		} else if (y >= DIALOG_TOPLEFT_Y && y <= DIALOG_BOTTOMLEFT_Y
				&& x < DIALOG_TOPLEFT_X) {
			window = win_blocks_west;
			y -= DIALOG_TOPLEFT_Y;
		} else if (y >= DIALOG_TOPLEFT_Y && y <= DIALOG_BOTTOMLEFT_Y
				&& x > DIALOG_TOPRIGHT_X) {
			window = win_blocks_east;
			y -= DIALOG_TOPLEFT_Y;
			x -= DIALOG_TOPRIGHT_X;
		} else {
			window = win_blocks_south;
			y -= DIALOG_BOTTOMLEFT_Y;
		}

		// Create the block
		blocks.push_back(Block(window, y, x, i));
	}
}

// Update dynamic content in the stats window
void update_stats() {
	std::string time_remaining = "???";
	std::string network_upload = "↑ " + formatByteCount(0) + "/s";
	std::string network_download = "↓ " + formatByteCount(0) + "/s";
	std::string disk_read = "↑ " + formatByteCount(0) + "/s";
	std::string disk_write = "↓ " + formatByteCount(0) + "/s";

	mvwaddstr(win_stats, 2, WIN_STATS_WIDTH - time_remaining.length(),
			time_remaining.c_str());
	mvwaddstr(win_stats, 3, WIN_STATS_WIDTH - network_upload.length(),
			network_upload.c_str());
	mvwaddstr(win_stats, 4, WIN_STATS_WIDTH - network_download.length(),
			network_download.c_str());
	mvwaddstr(win_stats, 5, WIN_STATS_WIDTH - disk_read.length(),
			disk_read.c_str());
	mvwaddstr(win_stats, 6, WIN_STATS_WIDTH - disk_write.length(),
			disk_write.c_str());

	wrefresh(win_stats);
}

void ui_main(std::size_t block_size, std::size_t device_size) {
	setlocale(LC_CTYPE, "");

	initscr();
	if (has_colors() == FALSE) {
		exit(1);
	}

	start_color();
	noecho();
	nonl();
	raw();

	keypad(stdscr, true);
	cbreak();

	curs_set(0);

	init_color(COLOR_BLACK, 188, 188, 188);

	if (COLS < MIN_TERM_WIDTH || LINES < MIN_TERM_HEIGHT) {
		return;
	}

	// Compute window geometry
	DIALOG_TOPLEFT_Y = (LINES - DIALOG_HEIGHT) / 2;
	DIALOG_TOPLEFT_X = (COLS - DIALOG_WIDTH) / 2;
	DIALOG_TOPRIGHT_Y = (LINES - DIALOG_HEIGHT) / 2;
	DIALOG_TOPRIGHT_X = (COLS + DIALOG_WIDTH) / 2;
	DIALOG_BOTTOMLEFT_Y = (LINES + DIALOG_HEIGHT) / 2;
	DIALOG_BOTTOMLEFT_X = (COLS - DIALOG_WIDTH) / 2;
	DIALOG_BOTTOMRIGHT_Y = (LINES + DIALOG_HEIGHT) / 2;
	DIALOG_BOTTOMRIGHT_X = (COLS + DIALOG_WIDTH) / 2;

	// Subtract dialog area from full screen area
	MAX_BLOCKS = (LINES * COLS) - (DIALOG_WIDTH * DIALOG_HEIGHT);

	unsigned int actual_blocks = device_size / block_size;

	// Increase scale factor until all blocks will fit
	while (device_size / block_size > (MAX_BLOCKS * BLOCK_FACTOR)) {
		BLOCK_FACTOR++;
	}

	BLOCK_COUNT = (device_size / block_size) / BLOCK_FACTOR;

	init_blocks();
	init_header();
	init_stats();

	for (int i = 0; i < BLOCK_COUNT; i++) {
		blocks[i].refresh();
	}

	// Start thread for hashing blocks
	//std::thread thread_hashing(snapshot_block_read, "");

	while (true) {
		update_stats();
		usleep(200000);
	}
}
