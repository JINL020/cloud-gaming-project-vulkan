/*
 *  UDPReceive.h
 *  MyReceiver
 *
 *  Created by Helmut Hlavacs on 11.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

extern "C" {
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


class UDPReceive {
	
public:
	int sock;
	struct sockaddr_in addr;
	char *recbuffer;
	bool leftover;
	
	UDPReceive();
	~UDPReceive(){ delete recbuffer; };
	void init( int port );
	int receive( char *buffer, int len, double *ptime  );
	int receive( char *buffer, int len, char *tag, double *ptime  );
	void closeSock();
};