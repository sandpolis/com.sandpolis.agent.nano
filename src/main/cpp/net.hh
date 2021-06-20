//============================================================================//
//                                                                            //
//                         Copyright © 2015 Sandpolis                         //
//                                                                            //
//  This source file is subject to the terms of the Mozilla Public License    //
//  version 2. You may not use this file except in compliance with the MPL    //
//  as published by the Mozilla Foundation.                                   //
//                                                                            //
//============================================================================//
#ifndef NET_H
#define NET_H

#include <errno.h>
#include <netdb.h>
#include <resolv.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

// The maximum number of bytes that a single protobuf varint can occupy
#define MAX_VARINT_WIDTH 5

namespace s7s::net {

	// Connect a socket to the remote host and return the file descriptor or -1 to
	// indicate failure.
	int OpenConnection(const std::string hostname, const int port);

	class Sock {

		// The socket file descriptor
		const int sockfd;

		// This endpoint's UUID
		const std::string uuid;

		// This endpoint's CVID
		int cvid;

		// The remote endpoint's CVID
		int remote_cvid;

		// The remote endpoint's UUID
		std::string remote_uuid;

		// Write a protobuf varint32 to the given buffer.
		void WriteVarint32(char *buffer, int value);

		// Read a protobuf varint32 from the given buffer.
		int ReadVarint32(char *buffer);

		// Determine the number of bytes that the value would occupy when encoded as a
		// protobuf varint32.
		int ComputeVarint32Width(int value);

	public:
		Sock(std::string _uuid, int sfd) : uuid(_uuid), sockfd(sfd) {
		}

		bool Connect();

		// Perform a CVID handshake with the remote host.
		bool CvidHandshake();

		// Serialize and send the given message to the remote host.
		bool Send(const core::net::MSG &msg);

		// Receive and parse a message from the remote host.
		bool Recv(core::net::MSG &msg);
	};
}

#endif
