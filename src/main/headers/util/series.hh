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
#ifndef SERIES_H
#define SERIES_H

#include <array>
#include <utility>
#include <iostream>
#include <unistd.h>
#include <chrono>

namespace s7s {

	// TimeSeries tracks statistics on a finite sequence of data points.
	template<typename T, int C>
	class TimeSeries {

		// A circular buffer containing the data points
		std::array<std::pair<std::uint64_t, T>, C> buffer;

		// The start index
		std::size_t start = 0;

		// The size of the buffer
		std::size_t size = 0;

	public:

		// Add a new data point at the current instant. An old point may be
		// overwritten, depending on how full the series is.
		void append(T datapoint) {
			const auto now = std::chrono::system_clock::now();

			buffer[start + size] = std::make_pair(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()), datapoint);
			if (size == C) {
				start = (start + 1) % C;
			}
			size++;
		}

		// Compute the rate between the last two data points.
		T get_last_rate() {
			if (size < 2) {
				// No data
			}

			std::pair<std::uint64_t, T> last1 = buffer[start + size - 1];
			std::pair<std::uint64_t, T> last2 = buffer[start + size - 2];

			T value_change = last1.second - last2.second;
			std::uint64_t time_changed = last1.first - last2.first;

			return value_change / time_changed;
		}

	};
}

#endif
