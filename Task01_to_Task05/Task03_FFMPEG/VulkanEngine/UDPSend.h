/*
 *  UDPSend.h
 *  MySender
 *
 *  Created by Helmut Hlavacs on 10.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


extern "C" {
#include <stdlib.h>
#include <sys/types.h>

#define NOMINMAX
#define _WINSOCK_DEPRECATED_NO_WARNINGS // dont emit warning on inet_addr call
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#include <ws2tcpip.h>
}

#ifndef UDPSEND_H
#define UDPSEND_H

namespace ve {
	class UDPSend {

	public:
		int sock;
		struct sockaddr_in addr;
		unsigned long packetnum;

		UDPSend();
		~UDPSend() { if (sock) closeSock(); };
		void init(char* address, int port);
		int send(uint8_t* buffer, int len);
		void closeSock();
	};
}
#endif




