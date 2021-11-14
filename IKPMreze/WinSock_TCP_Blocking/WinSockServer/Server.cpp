#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"

bool InitializeWindowsSockets();

int  main(void) 
{
    // Socket used for listening for new clients 
    SOCKET listenSocket = INVALID_SOCKET;
    // Socket used for communication with client
    SOCKET acceptedSocket = INVALID_SOCKET;
    // variable used to store function return value
    int iResult;
    // Buffer used for storing incoming data
    char recvbuf[DEFAULT_BUFLEN];
    
    if(InitializeWindowsSockets() == false)
    {
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
    }
    
    // Prepare address information structures
    addrinfo *resultingAddress = NULL;
    addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4 address
    hints.ai_socktype = SOCK_STREAM; // Provide reliable data streaming
    hints.ai_protocol = IPPROTO_TCP; // Use TCP protocol
    hints.ai_flags = AI_PASSIVE;     // 

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &resultingAddress);
    if ( iResult != 0 )
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    listenSocket = socket(AF_INET,      // IPv4 address famly
                          SOCK_STREAM,  // stream socket
                          IPPROTO_TCP); // TCP

    if (listenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(resultingAddress);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket - bind port number and local address 
    // to socket
    iResult = bind( listenSocket, resultingAddress->ai_addr, (int)resultingAddress->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(resultingAddress);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    unsigned long mode = 1;
    iResult = ioctlsocket(listenSocket, FIONBIO, &mode);
    if (iResult == SOCKET_ERROR) {
        printf("ioctlsocket error \n");
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

    printf("Server is listening ready to accept clients !!!\n ");


    // Since we don't need resultingAddress any more, free it
    freeaddrinfo(resultingAddress);

    fd_set readfds;
    FD_ZERO(&readfds);

    timeval timeVal;
    timeVal.tv_sec = 1;
    timeVal.tv_usec = 0;

    bool have = false;

    sockaddr_in client;
    int size = sizeof(client);

    char buffer[DEFAULT_BUFLEN];



    do {
        if(!have)
             FD_SET(listenSocket, &readfds);
        if (have)
            FD_SET(acceptedSocket, &readfds);

        iResult = select(0, &readfds, NULL, NULL, &timeVal);

        if (iResult == 0) {
            printf("Time expired\n");
            continue;
        }
        else if (iResult == SOCKET_ERROR) {
            printf("Error occupied closing listen socket \n");
            closesocket(listenSocket);
            break;
        }
        else if(FD_ISSET(listenSocket,&readfds))
        {
            printf("Accept clients \n");
            acceptedSocket = accept(listenSocket, (SOCKADDR*)&client, &size);

            if (acceptedSocket == INVALID_SOCKET) {
                printf("Invalid Accpet socket \n");
                closesocket(acceptedSocket);
                continue;
            }
            printf("Client accpeted ");
            iResult = ioctlsocket(acceptedSocket, FIONBIO, &mode);
            if (iResult == SOCKET_ERROR) {
                printf("ioctlsocket client error \n");
                closesocket(acceptedSocket);
                continue;
            }
            have = true;
            printf("Client is in non blocking mode");


        }
        else {
            printf("Client send something \n");
            iResult = recv(acceptedSocket, buffer, DEFAULT_BUFLEN, 0);
            buffer[iResult] = '\0';
            if (iResult == SOCKET_ERROR) {
                printf("recv error \n");
                continue;
            }

            printf("Data from %s ,  port : %d , data : %s  \n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), buffer);

        }
    } while (true);






















    // shutdown the connection since we're done
    iResult = shutdown(acceptedSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(acceptedSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(listenSocket);
    closesocket(acceptedSocket);
    WSACleanup();

    return 0;
}

bool InitializeWindowsSockets()
{
    WSADATA wsaData;
	// Initialize windows sockets library for this process
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return false;
    }
	return true;
}
