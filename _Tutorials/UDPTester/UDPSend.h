/*
 *  UDPSend.h
 *  MySender
 *
 *  Created by Helmut Hlavacs on 10.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

extern "C" {
#define NOMINMAX
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <in6addr.h>
#include <ws2ipdef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <WS2tcpip.h>
}

void startWinsock();

class UDPSend {
public:
	int sock = 0;
	struct sockaddr_in addr;
	unsigned long packetnum = 0;

	UDPSend(){};
	~UDPSend(){ if(sock) closeSock(); };
	void init( char *address, int port );
	int send( char *buffer, int len  );
	void closeSock();
};







