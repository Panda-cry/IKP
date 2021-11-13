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
#define SERVER_PORT1 15002
#define BUFFER_SIZE 512

int Read(SOCKET socket) {

	sockaddr_in client;
	int size = sizeof(client);
	char data[BUFFER_SIZE];
	int result = recvfrom(socket, data, BUFFER_SIZE, 0, (SOCKADDR*)&client, &size);

	if (result == SOCKET_ERROR) {
		printf("Error occupied \n");
		return -1;
	}
	data[result] = '\0';

	printf("Data send from client : %d , on port : %d , and data :%s \n", client.sin_addr, ntohs(client.sin_port), data);
	return 0;
}

int main() {

	sockaddr_in serverAddress;
	sockaddr_in serverAddress1;
	int size = sizeof(serverAddress);

	int iResult;

	char dataBuffer[BUFFER_SIZE];
	WSADATA wsadata;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
	//potrebno za sam start aplikacije 
	if (iResult != 0) {
		printf("Wsa start up error ! \n");
		return -1;
	}

	//popinjavanje sockaddr strukture poljima za server
	//popunjavamo ga memsetom jer jedan deo mora da budu samo 0 pa sve prvo setujemo
	//cast je char jer je to 1 bajt sto znaci najmanja jedinica u prog i svi bajtovi ce biti 0

	memset((char*)&serverAddress, 0, size);
	serverAddress.sin_family = AF_INET;//ipv4
	serverAddress.sin_addr.s_addr = INADDR_ANY; // sve ip adrese
	serverAddress.sin_port = htons(SERVER_PORT);//port

	memset((char*)&serverAddress1, 0, size);
	serverAddress1.sin_family = AF_INET;//ipv4
	serverAddress1.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS); // sve ip adrese
	serverAddress1.sin_port = htons(SERVER_PORT1);//port
	//kreiramo soket
	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (serverSocket == INVALID_SOCKET) {
		printf("invalid socket \n");
		WSACleanup();
		return -1;
	}

	SOCKET serverSocket1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (serverSocket1 == INVALID_SOCKET) {
		printf("invalid socket \n");
		WSACleanup();
		return -1;
	}
	// prebacujemo sokete u neblokirajui rezim
	unsigned long mode = 1;
	iResult = ioctlsocket(serverSocket, FIONBIO, &mode);
	if (iResult == SOCKET_ERROR) {
		printf("ioctalsocket first faulth \n");
		closesocket(serverSocket);
		closesocket(serverSocket1);
		return -1;
	}
	iResult = ioctlsocket(serverSocket1, FIONBIO, &mode);
	if (iResult == SOCKET_ERROR) {
		printf("ioctalsocket second faulth \n");
		closesocket(serverSocket);
		closesocket(serverSocket1);
		return -1;
	}
	//vezujemo serversku strukturu sa soketom // ip adresa + port
	iResult = bind(serverSocket, (SOCKADDR*)&serverAddress, size);

	if (iResult == SOCKET_ERROR) {
		printf("Invalid bind \n");
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}
	iResult = bind(serverSocket1, (SOCKADDR*)&serverAddress1, size);

	if (iResult == SOCKET_ERROR) {
		printf("Invalid bind \n");
		closesocket(serverSocket1);
		WSACleanup();
		return -1;
	}
	// namestanje vremena za koje ce cekati
	timeval timeVal;
	timeVal.tv_sec = 1;
	timeVal.tv_usec = 0;

	//kreiramo nase fd setove koji su kontejneri gde ce biti nasi soketi 
	//kada se select pozove ona izbaci soket koji je pozvao fju 
	fd_set readfds;
	fd_set writefsd;
	fd_set errorfds;
	FD_ZERO(&readfds);
	FD_ZERO(&writefsd);
	FD_ZERO(&errorfds);

	printf("UDP server is ready to recive information \n");

	while (true) {

		FD_SET(serverSocket, &readfds);
		FD_SET(serverSocket1, &readfds);

		FD_SET(serverSocket, &errorfds);
		FD_SET(serverSocket1, &errorfds);

		int result = select(0, &readfds, NULL, NULL, &timeVal);

		if (result == 0) {
			printf("Time expired \n");
			continue;
		}
		else if (result == SOCKET_ERROR) {
			printf("Error occupied \n");
			if (FD_ISSET(serverSocket, &errorfds)) {
				printf("error on socket 1 \n");
				closesocket(serverSocket);
			}
			else if (FD_ISSET(serverSocket1, &errorfds)) {
				printf("error on socket 1 \n");
				closesocket(serverSocket1);
			}
			return -1;
		}
		else
		{
			if (FD_ISSET(serverSocket, &readfds)) {
				Read(serverSocket);
			}
			else if (FD_ISSET(serverSocket1, &readfds)) {
				Read(serverSocket1);
			}
		}

		FD_CLR(serverSocket, &readfds);
		FD_CLR(serverSocket1, &readfds);
		FD_CLR(serverSocket1, &errorfds);
		FD_CLR(serverSocket1, &errorfds);
	}

	if (closesocket(serverSocket) == SOCKET_ERROR) {
		printf("closing socket error\n");
		WSACleanup();
		return -1;
	}
	if (closesocket(serverSocket1) == SOCKET_ERROR) {
		printf("closing socket error\n");
		WSACleanup();
		return -1;
	}
	WSACleanup();


	return 0;
}