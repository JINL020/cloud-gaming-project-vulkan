extern "C" {
#include<stdio.h>
#include<winsock2.h>
}

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc , char *argv[]){
    WSADATA wsa;
	SOCKET sock;
    struct sockaddr_in server;
    char *message , server_reply[2000];
	int recv_size;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0){// 2, 0 or 2,2??? 
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	printf("\nInitialised Winsock.\n");
	
    printf("\nCreating socket...\n");
		if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET){
			printf("Could not create socket : %d", WSAGetLastError());
			return 1;
		}
	printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr("::1");
	server.sin_family = AF_INET;
	server.sin_port = htons(50000);

	//Send some data
	message = "Hellooooo";
	if( send(sock , message , strlen(message) , 0) < 0){
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n"); 

    //Receive a reply from the server
	if((recv_size = recv(sock , server_reply , 2000 , 0)) == SOCKET_ERROR){
		puts("recv failed");
	}
	
	puts("Reply received\n");

	//Add a NULL terminating character to make it a proper string before printing
	server_reply[recv_size] = ' ';

	puts(server_reply);   

    closesocket(sock);
    WSACleanup();

	return 0;
}
