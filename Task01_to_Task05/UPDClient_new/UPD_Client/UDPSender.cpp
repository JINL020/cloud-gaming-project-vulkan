
//Task04-------------------Added by Me-------------------Task04//

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include "UDPSender.h"

extern "C" {
#include <stdio.h>
#include <time.h>
}

#define min(x,y)  ((x) <= (y) ? (x) : (y))
#define max(x,y)  ((x) >= (y) ? (x) : (y))



typedef struct RTHeader {
	double		  time;
	unsigned long packetnum;
	unsigned char fragments;
	unsigned char fragnum;
} RTHeader_t;

void UDPSender::startWinsock() {
	printf("Initialising Winsock...\n");
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0) {// 2, 0 or 2,2??? 
		printf("Failed. Error Code : %d", WSAGetLastError());
		return;
	}
	printf("Initialised Winsock.\n");
	return;
}


void UDPSender::init(const char* address, int port) {
	if (sock) {
		closeSock();
	}

	printf("\nCreating socket...");
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		return;
	}
	printf("\nSocket created\n");

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(address);
	addr.sin_port = htons(port);
}


int UDPSender::send(char* buffer, int len) {

	char sendbuffer[65000];

	packetnum++;

	if (len > 65000) {
		return 0;
	}
	//if( packetnum%100==50) return -1;		//test for packet loss

	int maxpacketsize = 1400;

	RTHeader_t header;
	header.time = clock() / (double)CLOCKS_PER_SEC;
	header.packetnum = packetnum;
	header.fragments = len / maxpacketsize;
	header.fragnum = 1;

	int left = len - header.fragments * maxpacketsize;
	if (left > 0) header.fragments++;

	int ret, bytes = 0;
	for (int i = 0; i < header.fragments; i++) {
		memcpy(sendbuffer, &header, sizeof(header));
		memcpy(sendbuffer + sizeof(header), buffer + bytes, min(maxpacketsize, len - bytes));

		ret = sendto(sock, sendbuffer, min(maxpacketsize, len - bytes) + sizeof(header), 0, (const struct sockaddr*)&addr, sizeof(addr));

		if (ret == -1) {
			printf("ret is -1; Error : %d", WSAGetLastError());
			return ret;
		}
		else {
			ret = ret - sizeof(header);
		}
		bytes += ret;
		header.fragnum++;
	}

	printf("\nSent data %d (size=%d)\n", buffer, len);

	return bytes;
}


void UDPSender::closeSock() {
	closesocket(sock);
	sock = 0;
}



//------------------------------------------------------------//

