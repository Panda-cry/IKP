// UDP client that uses blocking sockets

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "conio.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 15001
#define BUFFER_SIZE 512



int main() {

	sockaddr_in serverAddress;

	int size = sizeof(serverAddress);

	int iResult = 0;

	char dataBuffer[BUFFER_SIZE];

	WSADATA wsadata;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);

	if (iResult != 0) {
		printf("Wsa start up error \n");
		return -1;
	}
	char port[BUFFER_SIZE];

	//popunjavamo sockaddr strukturu 
	memset((char*)&serverAddress, 0, size);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
	printf("Enter port  to hit : \n");

	gets_s(port, BUFFER_SIZE);

	serverAddress.sin_port = htons(atoi(port));

	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (clientSocket == INVALID_SOCKET) {
		printf("invalid socket \n");
		WSACleanup();
		return -1;
	}
	
	
	while (true) {
		printf("Enter data to send : !!! \n");
		gets_s(dataBuffer, BUFFER_SIZE);

		iResult = sendto(clientSocket, dataBuffer, strlen(dataBuffer), 0, (SOCKADDR*)&serverAddress, size);

		if (iResult == SOCKET_ERROR) {
			printf("send to error \n");
			continue;
		}

		if (strcmp(dataBuffer, "end") == 0) {
			break;
		}


	}

	if (closesocket(clientSocket) == SOCKET_ERROR) {
		printf("closing socket error \n");
		WSACleanup();
		return -1;
	}
	WSACleanup();


	return 0;
}