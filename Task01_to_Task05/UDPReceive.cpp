/*
 *  UDPReceive.cpp
 *  MyReceiver
 *
 *  Created by Helmut Hlavacs on 11.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma comment(lib, "ws2_32.lib")


#include "UDPReceive.h"

extern "C" {
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <cstdint>
}

typedef struct RTHeader {
	double		  time;
	unsigned long packetnum;
	unsigned char fragments;
	unsigned char fragnum;
} RTHeader_t;


void startWinsock(){
    printf("Initialising Winsock...\n");
    WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0){// 2, 0 or 2,2??? 
		printf("Failed. Error Code : %d", WSAGetLastError());
		return;
	}
	printf("Initialised Winsock.\n");
    return;
}

UDPReceive::UDPReceive() {
	recbuffer = new char[65000];
}


void UDPReceive::init( int port ) {
    if(sock){
        closeSock();
    }

    printf("\nCreating socket...\n");
		if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET){
			printf("Could not create socket : %d", WSAGetLastError());
			return ;
		}
	printf("Socket created.\n");
	
	addr.sin_family = AF_INET;  
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons( port );
	int ret = bind(sock, (const sockaddr *)&addr, sizeof(addr));
	
	printf("\nBinding port %d return %d\n", port, ret );
	
	leftover=false;
}


int UDPReceive::receive( char *buffer, int len, double *ptime ) {
	return receive( buffer, len, "", ptime );
}

int UDPReceive::receive( uint8_t *buffer, int len, double *ptime ) {
	return receive( buffer, len, "", ptime );
}


int UDPReceive::receive( char *buffer, int len, char *tag, double *ptime ) {
	struct sockaddr_in si_other;
    socklen_t slen=sizeof(si_other);

	RTHeader_t *pheader = (RTHeader_t*)recbuffer;
	
	bool goon=true;
	int bytes = 0;
	int packetnum=-1;
	int fragments=-1;
	int fragnum=-1;
	int nextfrag=1;
	
	while( goon ) {
		
		int ret=0;
		
		if( !leftover ) {
			ret = recvfrom(sock, recbuffer, 65000, 0,(sockaddr*) &si_other, &slen);
		}
		leftover=false;
		
		//printf("%s UDP Packet %ld Size %d Fragment %d of %d Nextfrag %d\n", tag, pheader->packetnum, ret, pheader->fragnum, pheader->fragments, nextfrag );
		
		if( ret>sizeof( RTHeader_t ) ) {
			if( packetnum==-1 ) {						//first fragment of the new packet
				packetnum = pheader->packetnum;
			}
			
			if( packetnum != pheader->packetnum ) {		//last fragments lost
				printf("Last Frag %d lost", nextfrag );
				leftover = true;
				return -1;
			}

			//printf("%s UDP Packet %ld Size %d Fragment %d of %d Nextfrag %d\n", tag, pheader->packetnum, ret, pheader->fragnum, pheader->fragments, nextfrag );

			if( nextfrag!= pheader->fragnum ) {			//a fragment is missing
				printf("Fragment %d lost\n", nextfrag );
				return -1;
			}
			nextfrag++;
			
			memcpy( buffer+bytes, recbuffer + sizeof( RTHeader_t), ret - sizeof(RTHeader_t) );
			bytes += ret - sizeof( RTHeader_t );
			
			if(pheader->fragments == pheader->fragnum) goon=false;		//last fragment
			
			packetnum = pheader->packetnum;
			fragments = pheader->fragments;
			fragnum   = pheader->fragnum;
			
			*ptime = pheader->time;
			
		} else {
			printf("Fragment %d not larger than %zd", pheader->fragnum, sizeof( RTHeader_t ) );
			return -1;
		}
		
	}
	
	leftover = false;
	return bytes;
}

int UDPReceive::receive( uint8_t *buffer, int len, char *tag, double *ptime ) {
	struct sockaddr_in si_other;
    socklen_t slen=sizeof(si_other);

	RTHeader_t *pheader = (RTHeader_t*)recbuffer;
	
	bool goon=true;
	int bytes = 0;
	int packetnum=-1;
	int fragments=-1;
	int fragnum=-1;
	int nextfrag=1;
	
	while( goon ) {
		
		int ret=0;
		
		if( !leftover ) {
			ret = recvfrom(sock, recbuffer, 65000, 0,(sockaddr*) &si_other, &slen);
		}
		leftover=false;
		
		//printf("%s UDP Packet %ld Size %d Fragment %d of %d Nextfrag %d\n", tag, pheader->packetnum, ret, pheader->fragnum, pheader->fragments, nextfrag );
		
		if( ret>sizeof( RTHeader_t ) ) {
			if( packetnum==-1 ) {						//first fragment of the new packet
				packetnum = pheader->packetnum;
			}
			
			if( packetnum != pheader->packetnum ) {		//last fragments lost
				printf("Last Frag %d lost", nextfrag );
				leftover = true;
				return -1;
			}

			//printf("%s UDP Packet %ld Size %d Fragment %d of %d Nextfrag %d\n", tag, pheader->packetnum, ret, pheader->fragnum, pheader->fragments, nextfrag );

			if( nextfrag!= pheader->fragnum ) {			//a fragment is missing
				printf("Fragment %d lost\n", nextfrag );
				return -1;
			}
			nextfrag++;
			
			memcpy( buffer+bytes, recbuffer + sizeof( RTHeader_t), ret - sizeof(RTHeader_t) );
			bytes += ret - sizeof( RTHeader_t );
			
			if(pheader->fragments == pheader->fragnum) goon=false;		//last fragment
			
			packetnum = pheader->packetnum;
			fragments = pheader->fragments;
			fragnum   = pheader->fragnum;
			
			*ptime = pheader->time;
			
		} else {
			printf("Fragment %d not larger than %zd", pheader->fragnum, sizeof( RTHeader_t ) );
			return -1;
		}
		
	}
	
	leftover = false;
	return bytes;
}


void UDPReceive::closeSock() {
	closesocket(sock);
	sock = 0;
}

int main() {

	startWinsock();

	UDPReceive receiver;

	//receiver.init(50000);
    receiver.init(8088);

	while (true) {
		char buf[65000];
		double ptime;
		auto ret = receiver.receive(buf, sizeof buf, &ptime);
		buf[ret] = '\0';
		printf("Message: % s\n", buf);
	}

	WSACleanup();
}


