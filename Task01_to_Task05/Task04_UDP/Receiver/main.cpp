#include<stdio.h>
#include<winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS // dont emit warning on inet_addr call

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc , char *argv[])
{
	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in server;
	char *message , server_reply[2000];
	int recv_size;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
	}
	
	printf("Initialised.\n");
	
	//Create a socket
	if(sock = socket(PF_INET, SOCK_DGRAM, 0) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created.\n");
	
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8088);

	//Receive a reply from the server
	while((recv_size = recv(sock , server_reply , 2000 , 0)) == SOCKET_ERROR)
	{
		puts("recv failed");
	}
	
	puts("Reply received\n");

	//Add a NULL terminating character to make it a proper string before printing
    ;
	puts(server_reply);

	return 0;
}