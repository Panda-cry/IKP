

#include<stdio.h>
#include<winsock2.h>
#include<stdlib.h>

#define SERVER_PORT 15001
#define BUFFER_SIZE 512

int main() {

	sockaddr_in serverAddres;

	int sockAddrSize = sizeof(serverAddres);

	char dataBuffer[BUFFER_SIZE];

	int iResult;
	
	WSADATA wsaData;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSASTART UP failed with error : %d\n",WSAGetLastError());
		return -1;
	}

	//Kreiramo nas soket tj popunjavamo strukturu
	//koja ce presdtavljati nas soket
	//namestimo na 0 sve 
	memset((char*)&serverAddres, 0, sockAddrSize);
	serverAddres.sin_family = AF_INET;
	serverAddres.sin_addr.s_addr = INADDR_ANY;
	serverAddres.sin_port = htons(SERVER_PORT);

	//primamo sve moguce adrese htons port je zbog mreze 
	// a protokol familija je stavljen ipv4


	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (serverSocket == INVALID_SOCKET) {
		printf("Creating socket failed with error : %d /n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	//ako kreirani soket nije validan ocistimo zauzete resurse 
	// i izlazimo iz programa 

	iResult = bind(serverSocket, (SOCKADDR*)&serverAddres, sockAddrSize);
	
	//bajnudjemo nasu ip adresu tj konfiguraciju servera
	// na  sa socketom 

	if (iResult == SOCKET_ERROR) {
		printf("Binding didnt worked well !!!, : %d /n", WSAGetLastError());
		WSACleanup();
		return -1;
	}


	printf("Simple udp conection  starteda and waiting for clients to show up : !!! /n");


	int big_letters = 0 , small_letters = 0 , other_chars = 0;
	char previous[BUFFER_SIZE];

	while (1) {

		sockaddr_in clientAddres;
		memset((char*)&clientAddres, 0, sockAddrSize);

		memset(dataBuffer, 0, BUFFER_SIZE);

		iResult = recvfrom(serverSocket, dataBuffer, BUFFER_SIZE,
			0, (SOCKADDR*)&clientAddres,&sockAddrSize);

		if (iResult == SOCKET_ERROR) {
			printf("Error occupied while rcv from client !!! : %d /n", WSAGetLastError());
			continue;
		}
		
		dataBuffer[iResult] = '\0';

		char ipAddress[16];
		strcpy_s(ipAddress, sizeof(ipAddress), inet_ntoa(clientAddres.sin_addr));

		unsigned short clientPort = ntohs(clientAddres.sin_port);


		//Nesto se desi tj radi se nesto 

		for (int i = 0; i < strlen(dataBuffer); i++)
		{
			if (isupper(dataBuffer[i]))
				big_letters++;
			else if (islower(dataBuffer[i]))
				small_letters++;
			else
				other_chars++;
		}
		printf("Data from : %s , and port : %d , send data : %s , where big letters count : %d , small : %d  and other : %d \n",
			ipAddress, clientPort, dataBuffer, big_letters, small_letters, other_chars);

		iResult = sendto(serverSocket, dataBuffer, strlen(dataBuffer), 0,
			(SOCKADDR*)&clientAddres, sockAddrSize);
		printf("Sent \n");
		if (iResult == SOCKET_ERROR) {
			printf("send to failed  ! : %d /n", WSAGetLastError());
			continue;
		}

	}

	iResult = closesocket(serverSocket);
	if (iResult == SOCKET_ERROR) {
		printf("Close socket failed %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("SUCCESFULLY shut down . /n");
	WSACleanup();



	return 0;
}