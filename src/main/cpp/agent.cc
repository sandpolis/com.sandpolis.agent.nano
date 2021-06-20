//============================================================================//
//                                                                            //
//                         Copyright © 2015 Sandpolis                         //
//                                                                            //
//  This source file is subject to the terms of the Mozilla Public License    //
//  version 2. You may not use this file except in compliance with the MPL    //
//  as published by the Mozilla Foundation.                                   //
//                                                                            //
//============================================================================//
#include <chrono>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <thread>

#include "sock.hh"
#include "util/net.hh"
#include "util/resources.hh"
#include "util/uuid.hh"

int main(int argc, char **argv) {

	// Load build metadata from embedded resource file
	s7s::PropertiesResource so_build_resource;
	if (!s7s::resource_load("build.properties", so_build_resource)) {
		std::cerr << "Failed to parse build.properties" << std::endl;
		return 1;
	}

	// Load configuration from resource file
	s7s::PropertiesResource so_config_resource;
	if (!s7s::resource_load("agent.properties", so_config_resource)) {
		std::cerr << "Failed to parse agent.properties" << std::endl;
		return 1;
	}

	std::cout << "Launching Sandpolis Nano Agent ("
			<< so_build_resource.get_property("instance.version") << ")"
			<< std::endl;
	std::cout << "Build Platform: "
			<< so_build_resource.get_property("build.platform")
			<< std::endl;
	std::cout << "Build Timestamp: "
			<< so_build_resource.get_property("build.timestamp")
			<< std::endl;

	// Load UUID
	const std::string uuid = generate_uuid();

	// Begin connection routine
	long iteration = 0;

	for (;; iteration++) {

		s7s::net::Sock sock(uuid, server_address, server_port);

		if (sock.Connect()) {
			if (sock.CvidHandshake()) {
				// TODO enter sock event loop
				return 0;
			}
		}
		std::this_thread::sleep_for(
		std::chrono::milliseconds(loop_config.cooldown()));
	}
}
