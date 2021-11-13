
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

	int sockAddrSize = sizeof(serverAddress);

	char dataBuffer[BUFFER_SIZE];

	unsigned short serverPort = SERVER_PORT;

	WSADATA wsaData;

	int iResult = WSAStartup(MAKEWORD(2, 2, ), &wsaData);

	if (iResult != 0) {
		printf("Wsa start up is occupied an error ");
		return -1;
	}

	memset((char*)&serverAddress, 0, sockAddrSize);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPort);
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);


	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (clientSocket == INVALID_SOCKET) {
		printf("Error while creting client socket !!!");
		WSACleanup();
		return -1;
	}


	while (true) {

		printf("Enter message you want to send: \n ");
		gets_s(dataBuffer, BUFFER_SIZE);


		if (!strcmp(dataBuffer, "stop client")) {
			break;
		}
		
		iResult = sendto(clientSocket, dataBuffer, strlen(dataBuffer), 0,
			(SOCKADDR*)&serverAddress, sockAddrSize);

		if (iResult == SOCKET_ERROR) {
			printf("send to failed\n");
			closesocket(clientSocket);
			WSACleanup();
			return -1;
		}

		printf("I am witing from server to send me something \n");

		iResult = recvfrom(clientSocket, dataBuffer, BUFFER_SIZE, 0,
			(SOCKADDR*)&serverAddress, &sockAddrSize);

		if (iResult == SOCKET_ERROR) {
			printf("recfrom error \n");
			continue;
		}

		dataBuffer[iResult] = '\0';
		printf("Data recived from server:  %s \n", dataBuffer);
	}


	iResult = closesocket(clientSocket);
	if (iResult == SOCKET_ERROR) {
		printf("Error during closing \n");
		WSACleanup();
		return -1;
	}
	WSACleanup();




	return 0;
}