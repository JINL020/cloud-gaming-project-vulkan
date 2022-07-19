extern "C" {
#include <winsock2.h>
#include <WS2tcpip.h>
}

#ifndef UDPRECEIVE_H
#define UDPRECEIVE_H

class UDPReceiver {

public:
	int sock;
	struct sockaddr_in addr;
	char* recbuffer;
	bool leftover;

	UDPReceiver();
	~UDPReceiver();
	void startWinsock();
	void init(int port);
	int receive(char* buffer, int len, double* ptime);
	int receive(char* buffer, int len, const char* tag, double* ptime);
	void closeSock();
};

#endif