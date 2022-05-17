/*
 *  UDPSend.h
 *  MySender
 *
 *  Created by Helmut Hlavacs on 10.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "VEInclude.h"

#ifndef UDPSend_H
#define UDPSend_H

class UDPSend {

public:
	int sock;
	struct sockaddr_in addr;
	unsigned long packetnum;

	UDPSend();
	~UDPSend() { if (sock) close(sock); };
	void init(char* address, int port);
	int send(char* buffer, int len);
	void closeSock();
};

#endif



