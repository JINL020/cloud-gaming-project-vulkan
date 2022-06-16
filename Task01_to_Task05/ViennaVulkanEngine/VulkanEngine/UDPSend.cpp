
//Task04-------------------Added by Me-------------------Task04//
/*
#pragma comment(lib, "ws2_32.lib")

#include "UDPSend.h"

extern "C" {
#include <stdio.h>
#include <time.h>
}

#define min(x,y)  ((x) <= (y) ? (x) : (y))
#define max(x,y)  ((x) >= (y) ? (x) : (y))

namespace ve {

	typedef struct RTHeader {
		double		  time;
		unsigned long packetnum;
		unsigned char fragments;
		unsigned char fragnum;
	} RTHeader_t;


	void startWinsock() {
		printf("\nInitialising Winsock...");
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0) {// 2, 0 or 2,2??? 
			printf("Failed. Error Code : %d", WSAGetLastError());
			return;
		}
		printf("\nInitialised Winsock.\n");
		return;
	}


	void UDPSend::init(char* address, int port) {
		if (sock) {
			closeSock();
		}

		startWinsock();

		printf("\nCreating socket...\n");
		if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
			printf("Could not create socket : %d", WSAGetLastError());
			return;
		}
		printf("Socket created.\n");

		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(address);
		addr.sin_port = htons(port);
	}


	int UDPSend::send(char* buffer, int len) {

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
				;
				return ret;
			}
			else {
				ret = ret - sizeof(header);
			}
			bytes += ret;
			header.fragnum++;
		}

		puts("Data Send\n");

		return bytes;
	}

	int UDPSend::send(uint8_t* buffer, int len) {

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
				;
				return ret;
			}
			else {
				ret = ret - sizeof(header);
			}
			bytes += ret;
			header.fragnum++;
		}

		puts("Data Send\n");

		return bytes;
	}


	void UDPSend::closeSock() {
		closesocket(sock);
		sock = 0;
	}
}
*/

//------------------------------------------------------------//

