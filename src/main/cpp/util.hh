//============================================================================//
//                                                                            //
//                         Copyright © 2015 Sandpolis                         //
//                                                                            //
//  This source file is subject to the terms of the Mozilla Public License    //
//  version 2. You may not use this file except in compliance with the MPL    //
//  as published by the Mozilla Foundation.                                   //
//                                                                            //
//============================================================================//
#ifndef UTIL_H
#define UTIL_H

#include <random>
#include <string>
#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <algorithm>

namespace s7s::util {

	// Generate a new random UUID.
	std::string generate_uuid() {

		// Setup new random number generator on every call to save memory
		std::random_device device;
		std::mt19937 rng(device());
		std::uniform_int_distribution<int> dist(0, 15);

		const char *domain = "0123456789abcdef";

		std::string uuid;
		for (int i = 0; i < 8; ++i)
			uuid += domain[dist(rng)];
		uuid += "-";

		for (int i = 0; i < 4; ++i)
			uuid += domain[dist(rng)];
		uuid += "-";

		for (int i = 0; i < 4; ++i)
			uuid += domain[dist(rng)];
		uuid += "-";

		for (int i = 0; i < 4; ++i)
			uuid += domain[dist(rng)];
		uuid += "-";

		for (int i = 0; i < 12; ++i)
			uuid += domain[dist(rng)];

		return uuid;
	}

	const std::string exec(const char *cmd) {
		std::array<char, 128> buffer;
		std::string result;
		std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
		if (!pipe) {
			throw std::runtime_error("popen() failed!");
		}
		while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
			result += buffer.data();
		}

		// Trim whitespace
		result.erase(
				std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
					return !std::isspace(ch);
				}).base(), result.end());

		return result;
	}

}

#endif
