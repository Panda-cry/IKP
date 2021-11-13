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
#define SERVER_PORT 27016
#define BUFFER_SIZE 256

int main() {

	SOCKET connectSocket = INVALID_SOCKET;

	int iResult = 0;
	char dataBuffer[BUFFER_SIZE];

	WSADATA wsadata;

	if (WSAStartup(MAKEWORD(2, 2), &wsadata)) {
		printf("WSA start up error ! \n");
		WSACleanup();
		return -1;
	}

	connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connectSocket == INVALID_SOCKET) {
		printf("creation connect socket fail \n");
		closesocket(connectSocket);
		WSACleanup();
		return -1;
	}

	
	sockaddr_in serverAddress;
	int sockServersize = sizeof(serverAddress);
	memset((char*)&serverAddress, 0, sockServersize);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
	serverAddress.sin_port = htons(SERVER_PORT);


	iResult = connect(connectSocket, (SOCKADDR*)&serverAddress, sockServersize);

	if (iResult == SOCKET_ERROR) {
		printf("Connect failure \n");
		closesocket(connectSocket);
		WSACleanup();
		return -1;
	}

	do {
		printf("Enter message to send : \n");
		gets_s(dataBuffer, BUFFER_SIZE);

		iResult = send(connectSocket, dataBuffer,(int)strlen(dataBuffer),0);

		if (iResult == SOCKET_ERROR) {
			printf("send error \n");
			continue;
		}





	} while (true);

	return 0;
}