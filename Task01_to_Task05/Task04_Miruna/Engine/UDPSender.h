#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <string>
#include <iostream>

#undef APIENTRY
#define NOMINMAX
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

namespace ve {
	class UDPSender {

	public:
		int sock;
		struct sockaddr_in addr;
		unsigned long packetnum;

		UDPSender();
		~UDPSender() { if (sock) closesocket(sock); };
		int startWinsock(void);
		void init(char* address, int port);
		int send(char* buffer, int len);
		void closeSock();
	};

}

#endif