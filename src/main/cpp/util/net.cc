//============================================================================//
//                                                                            //
//                         Copyright © 2015 Sandpolis                         //
//                                                                            //
//  This source file is subject to the terms of the Mozilla Public License    //
//  version 2. You may not use this file except in compliance with the MPL    //
//  as published by the Mozilla Foundation.                                   //
//                                                                            //
//============================================================================//
#include "util/net.hh"

int OpenConnection(const std::string _hostname, const int _port) {
	const char *hostname = _hostname.c_str();
	char port[32];
	sprintf(port, "%d", _port);

	struct addrinfo hints = { 0 }, *addrs;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	const int status = getaddrinfo(hostname, port, &hints, &addrs);
	if (status != 0) {
		fprintf(stderr, "%s: %s\n", hostname, gai_strerror(status));
	}

	int sfd;
	for (struct addrinfo *addr = addrs; addr != nullptr; addr = addr->ai_next) {
		sfd = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
		if (sfd == -1) {
			continue;
		}

		if (connect(sfd, addr->ai_addr, addr->ai_addrlen) == 0) {
			// Connection success
			break;
		}

		close(sfd);
		sfd = -1;
	}

	freeaddrinfo(addrs);
	return sfd;
}
