/*
 *  UDPReceive6.cpp
 *
 *  Created by Helmut Hlavacs (2022).
 *
 */


#pragma comment(lib, "ws2_32.lib")


#include "UDPReceive6.h"

extern "C" {
#include <stdio.h>
#include <time.h>
}


typedef struct RTHeader {
	double		  time;
	unsigned long packetnum;
} RTHeader_t;


int startWinsock(void)
{
	WSADATA wsa;
	auto err = WSAStartup(MAKEWORD(2, 2), &wsa);
	return err;
}


UDPReceive6::UDPReceive6() {
	recbuffer = new char[65000];
}


void UDPReceive6::init(int port) {
	sock = socket(AF_INET6, SOCK_DGRAM, 0);

	memset(&addr, 0, sizeof(addr));

	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(port);
	addr.sin6_addr = in6addr_any;
	auto ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	//printf("Binding port %d return %d errno %d\n", port, ret, WSAGetLastError());
}


int UDPReceive6::receive(char* buffer, int len, double* ptime) {
	struct sockaddr_in6 si_other;
	socklen_t slen = sizeof(si_other);

	RTHeader_t* pheader = (RTHeader_t*)recbuffer;

	while (true) {
		auto ret = recvfrom(sock, recbuffer, 65000, 0, (sockaddr*)&si_other, &slen);

		if (ret > sizeof RTHeader_t) {
			memcpy(buffer, recbuffer + sizeof RTHeader_t, ret - sizeof RTHeader_t);
			return ret - sizeof RTHeader_t;
		}
	}
	return 0;
}


void UDPReceive6::closeSock() {
	closesocket(sock);
	sock = 0;
}



int main() {

	startWinsock();

	UDPReceive6 receiver;

	receiver.init(50000);

	while (true) {
		char buf[65000];
		double ptime;
		auto ret = receiver.receive(buf, sizeof buf, &ptime);
		buf[ret] = '\0';
		printf("Message: % s\n", buf);
	}

	WSACleanup();
}



