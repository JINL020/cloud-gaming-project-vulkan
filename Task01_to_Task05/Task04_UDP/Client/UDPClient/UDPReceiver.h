#pragma once
/*
 *  UDPReceive.h
 *  MyReceiver
 *
 *  Created by Helmut Hlavacs on 11.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


extern "C" {
#include <WinSock2.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ws2tcpip.h>
}


class UDPReceiver {

public:
	int sock;
	struct sockaddr_in server;
	char* recbuffer;
	bool leftover;

	UDPReceiver();
	~UDPReceiver() { delete recbuffer; };

	void startWinsock();
	void createSocket();
	void init(int port);
	int receive(char* buffer, int len, double* ptime);
	int receive(char* buffer, int len, char* tag, double* ptime);
	void closeSock();
};