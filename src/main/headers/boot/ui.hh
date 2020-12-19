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
#ifndef UI_H
#define UI_H

#include <ncursesw/ncurses.h>
#include <wctype.h>
#include <wchar.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <thread>

// The smallest terminal that is supported
#define MIN_TERM_WIDTH 16
#define MIN_TERM_HEIGHT 16

// The overall size of the center dialog
#define DIALOG_WIDTH 89
#define DIALOG_HEIGHT 18

// The size of the header window
#define WIN_HEADER_WIDTH 89
#define WIN_HEADER_HEIGHT 9

// The size of the stats window
#define WIN_STATS_WIDTH 89
#define WIN_STATS_HEIGHT 9

#define BLOCK_UNSEEN L"."
#define BLOCK_SYNCED L"▣"
#define BLOCK_HASHED L"▢"

void ui_main(size_t block_size, size_t device_size);

enum BlockState {
	unseen, hashing, syncing, hashed, synced
};

class Block {
private:
	BlockState state;
	WINDOW *window;
	const int y;
	const int x;
	const unsigned int i;

public:
	Block(WINDOW *_window, int _y, int _x, unsigned int _i) : window(_window), y(
			_y), x(_x), i(_i), state(unseen) {
	}

	void refresh() {

		switch (state) {
		case unseen:
			mvwaddwstr(window, y, x, BLOCK_UNSEEN);
			wrefresh(window);
			break;
		case hashed:
			mvwaddwstr(window, y, x, BLOCK_HASHED);
			wrefresh(window);
			break;
		case synced:
			mvwaddwstr(window, y, x, BLOCK_SYNCED);
			wrefresh(window);
			break;
		}
	}

	void set_state(BlockState _state) {
		state = _state;
	}
};

#endif
