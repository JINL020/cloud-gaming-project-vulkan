
//Task04-------------------Added by Me-------------------Task04//

extern "C" {
#include <sys/types.h>

#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library
}

#ifndef UDPSEND_H
#define UDPSEND_H

namespace ve {
	int startWinsock(void);

	class UDPSend {

	public:
		int sock = 0;
		struct sockaddr_in addr;
		unsigned long packetnum = 0;

		UDPSend();
		~UDPSend() { if (sock) closeSock(); };
		void init(char* address, int port);
		int send(uint8_t* buffer, int len);
		void closeSock();
	};
}
#endif

//------------------------------------------------------------//