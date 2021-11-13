// UDPVezbe5Mreze1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

#define SERVER_PORT 15001
#define BUFFER_SIZE 512		


int main()
{
	sockaddr_in  serverAddress;

	int serverAddressSize = sizeof(serverAddress);
	int iResult = 0;
	char dataBuffer[BUFFER_SIZE];

	WSADATA wsaData;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAstartup error is occupied !!!  %d /n", WSAGetLastError());
		return -1;
	}

	//inicijalizacija naseg servera
	
	memset((char*)&serverAddress, 0, serverAddressSize);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(SERVER_PORT);


	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (serverSocket == INVALID_SOCKET) {

		printf("Error occupied while creating socket !!! : %d \n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	iResult = bind(serverSocket, (SOCKADDR*)&serverAddress, serverAddressSize);

	if (iResult == SOCKET_ERROR) {
		printf("Error ocupied while binding %d \n", WSAGetLastError());
		closesocket(serverSocket);
		WSACleanup();
		return -1;

	}

	printf(" Simple UDP server is created  : and he waiting for client to send something  \n");

	char previous[BUFFER_SIZE] = "";
	while (1) {
		sockaddr_in clientSide;
		iResult = recvfrom(serverSocket, dataBuffer, BUFFER_SIZE, 0, (SOCKADDR*)&clientSide, &serverAddressSize);
		if (iResult == SOCKET_ERROR) {
			printf("Error during recvfrom \n");
			continue;
		}
		dataBuffer[iResult] = '\0';
		printf("Server recived something from client  : %s  : and port %d \n", inet_ntoa(clientSide.sin_addr), ntohs(clientSide.sin_port));

		int lettersUp = 0;
		int letterDown = 0;
		int letterAscii = 0;
		for (int i = 0; i < strlen(dataBuffer); i++)
		{
			if (isupper(dataBuffer[i]))
				lettersUp++;
			if (islower(dataBuffer[i]))
				letterDown++;
			if (isalpha(dataBuffer[i]))
				letterAscii++;
		}



		printf("Data size is : %d , Letters up : %d , Letters down  : %d , Alphanumeric :  %d , and whole data is : %s \n", strlen(dataBuffer), lettersUp, letterDown, letterAscii, dataBuffer);
		if (strcmp(dataBuffer,previous) == 0) {
			printf("Client send 2 same datas ");
			closesocket(serverSocket);
			WSACleanup();
			return 0;

		}
		else {
			strcpy_s(previous, dataBuffer);
		}

		char server[BUFFER_SIZE];

		sprintf_s(server, "Server recived : %s", dataBuffer);

		iResult = sendto(serverSocket, server, strlen(server), 0, (SOCKADDR*)&clientSide, serverAddressSize);

		if (iResult == SOCKET_ERROR) {
			printf("send to error");
			continue;
		}
		printf("\n Server sucesffully send data to client.\n");



	}

	iResult = closesocket(serverSocket);
	if (iResult == SOCKET_ERROR) {
		printf("Error occupied wihile releasing server socekt  !");
		WSACleanup();
		return -1;
	}

	WSACleanup();





	return 0;
}
