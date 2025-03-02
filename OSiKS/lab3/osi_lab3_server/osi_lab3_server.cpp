#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib,"Ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

const int USERNAME_LENGTH = 256;
const int MESSAGE_BUFFER_SIZE = 1000;
const int MAX_CLIENTS = 5;
int currentClientCount;
char clientUsernames[MAX_CLIENTS + 1][USERNAME_LENGTH];

SOCKET serverSocket;
SOCKET clientSockets[MAX_CLIENTS];
SOCKADDR_IN clientAddresses[MAX_CLIENTS];
USHORT clientPorts[MAX_CLIENTS];

DWORD WINAPI HandleClientCommunication(LPVOID clientSocket)
{
    int result;
    char receivedMessage[MESSAGE_BUFFER_SIZE];
    char responseMessage[MESSAGE_BUFFER_SIZE];
    int i, j, currentClientIndex;

    SOCKET clientSocketHandle;
    clientSocketHandle = *((SOCKET*)clientSocket);

    while (true)
    {
        result = recv(clientSocketHandle, receivedMessage, MESSAGE_BUFFER_SIZE, 0);

        if (result == SOCKET_ERROR)
        {
            cout << "Error: Unable to receive message!" << endl;
            closesocket(clientSocketHandle);
            cout << "Connection closed." << endl;
            return SOCKET_ERROR;
        }
        else
        {
            if (result >= MESSAGE_BUFFER_SIZE) result = MESSAGE_BUFFER_SIZE - 1;
            receivedMessage[result] = '\0';
        }

        SOCKADDR_IN clientAddress;
        for (i = 0; i < currentClientCount; i++)
        {
            if (clientSockets[i] == clientSocketHandle)
            {
                clientAddress = clientAddresses[i];
            }
        }

        currentClientIndex = 0;
        while (clientPorts[currentClientIndex] != clientAddress.sin_port)
        {
            currentClientIndex++;
        }

        if (!strcmp(receivedMessage, "/exit"))
        {
            responseMessage[0] = '\0';
            strcat_s(responseMessage, clientUsernames[currentClientIndex]);
            strcat_s(responseMessage, " left the chat.");

            for (i = 0; i < currentClientCount; i++)
            {
                if (clientSockets[i] != clientSocketHandle) result = send(clientSockets[i], responseMessage, MESSAGE_BUFFER_SIZE, 0);
            }

            if (result == SOCKET_ERROR)
            {
                cout << "Error: Unable to send message!" << endl;
                return SOCKET_ERROR;
            }

            cout << "Client disconnected." << endl;
            closesocket(clientSocketHandle);
            cout << "Connection closed." << endl;

            for (j = currentClientIndex; j < currentClientCount; j++)
            {
                clientSockets[j] = clientSockets[j + 1];
                clientAddresses[j] = clientAddresses[j + 1];
                clientPorts[j] = clientPorts[j + 1];
                strcpy_s(clientUsernames[j], clientUsernames[j + 1]);
            }

            clientSockets[currentClientCount - 1] = SOCKET_ERROR;
            currentClientCount--;
            cout << "Current online: " << currentClientCount << endl;
            return SOCKET_ERROR;
        }

        if (receivedMessage[0] != '\0')
        {
            cout << clientUsernames[currentClientIndex] << ": " << receivedMessage << endl;
            responseMessage[0] = '\0';
            strcat_s(responseMessage, clientUsernames[currentClientIndex]);
            strcat_s(responseMessage, ": ");
            strcat_s(responseMessage, receivedMessage);

            for (i = 0; i < currentClientCount; i++)
            {
                if (clientSockets[i] != clientSocketHandle) result = send(clientSockets[i], responseMessage, MESSAGE_BUFFER_SIZE, 0);
            }

            if (result == SOCKET_ERROR)
            {
                cout << "Error: Unable to send message!" << endl;
                return SOCKET_ERROR;
            }
        }
    }
}

int main()
{
    int result;
    int i;
    char newClientMessage[MESSAGE_BUFFER_SIZE];

    currentClientCount = 0;

    WORD socketVersion;
    WSADATA wsaData;
    socketVersion = MAKEWORD(2, 2);
    WSAStartup(socketVersion, &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Error: Unable to create socket!" << endl;
        WSACleanup();
        system("pause");
        return SOCKET_ERROR;
    }

    for (i = 0; i < MAX_CLIENTS; i++)
    {
        clientSockets[i] = SOCKET_ERROR;
    }

    SOCKADDR_IN serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(2008);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    result = bind(serverSocket, (LPSOCKADDR)&serverAddress, sizeof(serverAddress));

    if (result == SOCKET_ERROR)
    {
        cout << "Error: Unable to bind socket!" << endl;
        WSACleanup();
        system("pause");
        return SOCKET_ERROR;
    }

    cout << "Server started at port: " << htons(serverAddress.sin_port) << endl;

    while (true)
    {
        result = listen(serverSocket, 10);

        if (result == SOCKET_ERROR)
        {
            cout << "Error: Cannot listen to socket!" << endl;
            WSACleanup();
            system("pause");
            return SOCKET_ERROR;
        }

        SOCKET clientSocketHandle;
        SOCKADDR_IN clientAddress;
        int clientAddressLength = sizeof(clientAddress);

        clientSocketHandle = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);

        if (clientSocketHandle == INVALID_SOCKET)
        {
            cout << "Error: Unable to accept socket!" << endl;
            WSACleanup();
            system("pause");
            return SOCKET_ERROR;
        }

        cout << "New connection: " << inet_ntoa(clientAddress.sin_addr) << ":" << htons(clientAddress.sin_port) << endl;
        cout << "Users: " << currentClientCount + 1 << endl;

        result = recv(clientSocketHandle, clientUsernames[currentClientCount], USERNAME_LENGTH, 0);

        if (result == SOCKET_ERROR)
        {
            cout << "Error: Unable to receive message!" << endl;
            system("pause");
            return SOCKET_ERROR;
        }

        if (!strcmp(clientUsernames[currentClientCount], "/shutdown"))
        {
            for (i = 0; i < currentClientCount; i++)
            {
                result = send(clientSockets[i], "Server shutdown.", MESSAGE_BUFFER_SIZE, 0);
                closesocket(clientSockets[i]);
            }

            result = send(clientSocketHandle, "Server shutdown.", MESSAGE_BUFFER_SIZE, 0);
            closesocket(clientSocketHandle);
            break;
        }
        else
        {
            if (currentClientCount < MAX_CLIENTS)
            {
                clientPorts[currentClientCount] = clientAddress.sin_port;
                clientSockets[currentClientCount] = clientSocketHandle;
                clientAddresses[currentClientCount] = clientAddress;
                newClientMessage[0] = '\0';

                strcat_s(newClientMessage, "Client ");
                strcat_s(newClientMessage, clientUsernames[currentClientCount]);
                strcat_s(newClientMessage, " connected (IP: ");
                strcat_s(newClientMessage, inet_ntoa(clientAddress.sin_addr));
                strcat_s(newClientMessage, ")");
                cout << newClientMessage << endl;

                currentClientCount++;

                for (i = 0; i < currentClientCount; i++)
                {
                    if (clientSockets[i] != clientSocketHandle) result =
                        send(clientSockets[i], newClientMessage, MESSAGE_BUFFER_SIZE, 0);
                }
            }
            else
            {
                cout << "Max online." << endl;
                result = send(clientSocketHandle, "Server is full, please try again later.", USERNAME_LENGTH, 0);
                closesocket(clientSocketHandle);
                cout << "Connection closed." << endl;
            }

            DWORD threadID;
            CreateThread(NULL, NULL, HandleClientCommunication, &clientSocketHandle, NULL, &threadID);
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}