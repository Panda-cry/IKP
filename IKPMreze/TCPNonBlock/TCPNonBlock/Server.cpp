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

#define SERVER_PORT 27016
#define BUFFER_SIZE 256
#define CLIENT_MAX 3

struct Data {
	char name[15];
	char last[20];
	int number;
};

int main() {

	SOCKET listenSocket = INVALID_SOCKET;

	int iResult = 0;

	WSADATA wsadata;

	if (WSAStartup(MAKEWORD(2, 2), &wsadata)) {
		printf("wsa start up  error \n");
		return -1;
	}

	sockaddr_in serverAddress;
	int size = sizeof(serverAddress);
	memset((char*)&serverAddress, 0, size);
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(SERVER_PORT);

	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (listenSocket == INVALID_SOCKET) {
		printf("creation socket error \n");
		WSACleanup();
		return  -1;
	}

	iResult = bind(listenSocket, (SOCKADDR*)&serverAddress, size);

	if (iResult == SOCKET_ERROR) {
		printf("bind error \n");
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}

	unsigned long mode = 1;
	iResult = ioctlsocket(listenSocket, FIONBIO, &mode);

	if (iResult == SOCKET_ERROR) {
		printf("ioctalsocket error \n");
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}

	iResult = listen(listenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR) {
		printf("listen error \n");
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}

	printf("Server is listening ready to accept clients !!! 3 is maxx ");

	fd_set readfsd;
	FD_ZERO(&readfsd);

	timeval timeVal;
	timeVal.tv_sec = 3;
	timeVal.tv_usec = 0;

	sockaddr_in client[CLIENT_MAX];
	
	SOCKET clients[CLIENT_MAX];
	memset((char*)&clients, 0, sizeof(SOCKET) * CLIENT_MAX);

	int currentIndexClients = 0;

	char dataBuffer[BUFFER_SIZE];

	while (true) {

		if(currentIndexClients != CLIENT_MAX)
		FD_SET(listenSocket, &readfsd);

		for (int i = 0; i < currentIndexClients; i++) {
			FD_SET(clients[i], &readfsd);
		}

		iResult = select(0, &readfsd, NULL, NULL, &timeVal);

		if (iResult == SOCKET_ERROR) {
			printf("Error occupied ");
			closesocket(listenSocket);
			WSACleanup();
			return -1;
		}
		else if (iResult == 0) {
			printf("Time expired \n");
			continue;
		}
		else if (FD_ISSET(listenSocket, &readfsd)) {
			iResult = accept(clients[currentIndexClients], (SOCKADDR*)&client[currentIndexClients], &size);
			if (iResult == INVALID_SOCKET) {
				printf("Error on %d", currentIndexClients);
				closesocket(clients[currentIndexClients]);
				continue;
			}
			printf("Added client to array \n");
			iResult = ioctlsocket(clients[currentIndexClients], FIONBIO, &mode);
			if (iResult == SOCKET_ERROR) {
				printf("ioctalsocket error client \n");
				closesocket(clients[currentIndexClients]);
				continue;
			}
			currentIndexClients++;
		}
		else {
			for (int i = 0; i < currentIndexClients; i++) {
				if (FD_ISSET(clients[i], &readfsd)) {
					iResult = recv(clients[i], dataBuffer, BUFFER_SIZE, 0);
					if (iResult > 0) {
						printf("Data from : %s , and port : %d  \n",
							inet_ntoa(client[i].sin_addr), ntohs(client[i].sin_port));
						dataBuffer[iResult] = '\0';
						printf("Message received from client (%d):\n", i + 1);

						//primljenoj poruci u memoriji pristupiti preko pokazivaca tipa (studentInfo *)
						//jer znamo format u kom je poruka poslata a to je struct studentInfo
						Data* student = (Data*)dataBuffer;

						printf("Ime i prezime: %s %s  \n", student->name, student->last);

						printf("Poeni studenta: %d  \n", ntohs(student->number));
						printf("_______________________________  \n");

					}
					else if (iResult == 0) {
						printf("current client closed up  %d \n", i);
						closesocket(clients[i]);
						for (int j = i; j < currentIndexClients - 1; j++) {
							clients[j] = clients[j + 1];
						}
						currentIndexClients--;

					}
					else {
						printf("Error %d ", i);
						closesocket(clients[i]);
						for (int j = i; j < currentIndexClients - 1; j++) {
							clients[j] = clients[j + 1];
						}
						currentIndexClients--;
					}
			



				}





			}


		}


	
	}
	closesocket(listenSocket);
	WSACleanup();

	return 0;
}