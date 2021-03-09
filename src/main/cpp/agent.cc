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

	//core::instance::AgentConfig config;

	// Load build metadata from resource file
	s7s::PropertiesResource so_build_resource;
	if (!s7s::resource_load("build.properties", so_build_resource)) {
		return 1;
	}

	// Load configuration from resource file
	s7s::RawResource so_config_resource;
	if (!s7s::resource_load("agent.config", so_config_resource)) {
		return 1;
	}
	/*if (!config.ParseFromArray(so_config_resource.data(),
	so_config_resource.length())) {
	std::cout << "Failed to read embedded configuration!" << std::endl;
	return 1;
	}*/

	std::cout << "Launching Sandpolis Micro Agent ("
			<< so_build_resource.get_property("instance.version") << ")"
			<< std::endl;
	std::cout << "Build Environment:" << std::endl;
	std::cout << "   Platform: "
			<< so_build_resource.get_property("build.platform") << std::endl;
	std::cout << "     Gradle: "
			<< so_build_resource.get_property("build.gradle.version")
			<< std::endl;
	std::cout << "       Java: "
			<< so_build_resource.get_property("build.java.version")
			<< std::endl;

	// Load UUID
	std::string uuid = generate_uuid();

	// Begin connection routine
	long iteration = 0;
	/*const core::instance::LoopConfig &loop_config =
	config.network().loop_config();
	while (iteration < loop_config.iteration_limit()
	|| loop_config.iteration_limit() == 0) {

	for (int i = 0; i < loop_config.target_size(); ++i) {
	std::cout << "Attempting connection: "
	<< loop_config.target(i).address() << std::endl;
	int sfd = OpenConnection(loop_config.target(i).address(),
	loop_config.target(i).port());
	if (sfd > 0) {
	Sock sock(uuid, sfd);

	if (sock.CvidHandshake()) {
	// TODO enter sock event loop
	return 0;
	}

	iteration = 0;
	break;
	}

	iteration++;
	std::this_thread::sleep_for(
	std::chrono::milliseconds(loop_config.cooldown()));
	}
	}*/
}
