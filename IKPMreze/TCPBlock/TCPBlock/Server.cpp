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

	SOCKET listenSocket = INVALID_SOCKET;

	SOCKET acceptSocket1 = INVALID_SOCKET;
	SOCKET acceptSocket2 = INVALID_SOCKET;

	char dataBuffer1[BUFFER_SIZE];
	char dataBuffer2[BUFFER_SIZE];

	WSADATA wsadata;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);

	if (iResult != 0) {
		printf("WSa start up error ! \n");
		return -1;
	}

	sockaddr_in serverAddress;

	int sockAddrsize = sizeof(serverAddress);

	memset((char*)&serverAddress, 0, sockAddrsize);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(SERVER_PORT);
 

	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("error during creating socket \n");
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}
	iResult = bind(listenSocket, (SOCKADDR*)&serverAddress, sockAddrsize);
	if (iResult == SOCKET_ERROR) {
		printf("Error during listeening socket ! \n");
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}



	iResult = listen(listenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR) {
		printf("Error during listeening socket ! \n");
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}

	printf("Server is listening ... \n");

	do {
		sockaddr_in clientSide;
		int sockClientSize = sizeof(clientSide);
		acceptSocket1 = accept(listenSocket, (struct sockaddr*)&clientSide,&sockClientSize);

		if (acceptSocket1 == INVALID_SOCKET) {
			printf("accept 1 error");
			closesocket(listenSocket);
			WSACleanup();
			return -1;
		}

		printf("First client accepted: on address : %d , and port : %d  ", inet_ntoa(clientSide.sin_addr), ntohs(clientSide.sin_port));


		/*acceptSocket2 = accept(listenSocket, (struct sockaddr*)&clientSide, &sockClientSize);

		if (acceptSocket2 == INVALID_SOCKET) {
			printf("accept 1 error");
			closesocket(listenSocket);
			closesocket(acceptSocket1);
			WSACleanup();
			return -1;
		}

		printf("Second client accepted: on address : %d , and port : %d  ", inet_ntoa(clientSide.sin_addr), ntohs(clientSide.sin_port));
*/

		printf("Server is waiting on infos from thoose two  !!! \n");
		do {

			iResult = recv(acceptSocket1, dataBuffer1, BUFFER_SIZE, 0);
			if (iResult == SOCKET_ERROR) {
				printf("recv error \n");
				continue;
			}
			else if (iResult == 0)	// Check if shutdown command is received
			{
				// Connection was closed successfully
				printf("Connection with first client closed.\n");
				
				closesocket(acceptSocket1);
				
				break;
			}
			else	// There was an error during recv
			{

				printf("recv failed with error: %d\n", WSAGetLastError());
				shutdown(acceptSocket1, SD_BOTH);
				closesocket(acceptSocket1);
				closesocket(acceptSocket2);
				break;
			}
			dataBuffer1[iResult] = '\0';
			printf("Data get from client 1 : %s", dataBuffer1);



		} while (true);



	} while (true);

	return 0;
}