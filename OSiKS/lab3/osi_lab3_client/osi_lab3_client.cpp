#pragma comment (lib,"Ws2_32.lib")
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string>
using namespace std;

bool isOffline;
const int USERNAME_LENGTH = 256;
const int MESSAGE_BUFFER_SIZE = 1000;

DWORD WINAPI ReceiveMessages(LPVOID clientSocket)
{
    int result;
    char response[MESSAGE_BUFFER_SIZE];

    SOCKET clientSocketHandle;
    clientSocketHandle = *((SOCKET*)clientSocket);

    result = recv(clientSocketHandle, response, MESSAGE_BUFFER_SIZE, 0);

    if (!strcmp(response, "Server shutting down."))
    {
        cout << "Server shutting down." << endl;
        isOffline = true;
        return 0;
    }

    if (!strcmp(response, "Server is full. Try again later."))
    {
        cout << "Server is full. Try again later." << endl;
        isOffline = true;
        return 0;
    }

    if (!isOffline)
    {
        if (result == SOCKET_ERROR)
        {
            result = 0;
            cout << "Error: Failed to receive message!" << endl;
            isOffline = true;
            return 0;
        }
        else
        {
            cout << response << endl;
        }
    }
    return 1;
}

DWORD WINAPI SendMessages(LPVOID clientSocket)
{
    int result;
    char buffer[MESSAGE_BUFFER_SIZE];
    SOCKET clientSocketHandle;
    clientSocketHandle = *((SOCKET*)clientSocket);
    gets_s(buffer);

    if (!strcmp(buffer, "/exit"))
    {
        isOffline = true;
        result = send(clientSocketHandle, buffer, MESSAGE_BUFFER_SIZE, 0);
        return 0;
    }
    else
    {
        result = send(clientSocketHandle, buffer, MESSAGE_BUFFER_SIZE, 0);

        if (result == SOCKET_ERROR)
        {
            cout << "Error: Failed to send message!" << endl;
            WSACleanup();
            system("pause");
            return 0;
        }
    }
    return 1;
}

int main()
{
    string serverIP;
    WSADATA wsaData;
    int result = 0;
    isOffline = false;
    char username[USERNAME_LENGTH];
    WORD version = MAKEWORD(2, 2);

    WSAStartup(version, (LPWSADATA)&wsaData);
    SOCKET clientSocketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocketHandle == SOCKET_ERROR)
    {
        cout << "Error: Failed to create socket!" << endl;
        WSACleanup();
        system("pause");
        return 1;
    }

    cout << "IP > ";
    cin >> serverIP;
    cin.ignore();

    SOCKADDR_IN serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(2008);
    inet_pton(AF_INET, serverIP.c_str(), &serverAddress.sin_addr);

    result = connect(clientSocketHandle, (LPSOCKADDR)&serverAddress, sizeof(serverAddress));

    if (result == SOCKET_ERROR)
    {
        cout << "Error: Failed to connect to the server!" << endl;
        WSACleanup();
        system("pause");
        return 1;
    }

    cout << "Connected!" << endl;
    cout << "Username: ";
    cin >> username;

    result = send(clientSocketHandle, username, USERNAME_LENGTH, 0);

    if (result == SOCKET_ERROR)
    {
        cout << "Error: Failed to send username!" << endl;
        WSACleanup();
        return 1;
    }

    cout << "Welcome to the chat room. Use /exit to leave." << endl;

    while (!isOffline)
    {
        DWORD threadID;
        CreateThread(NULL, NULL, SendMessages, &clientSocketHandle, NULL, &threadID);
        CreateThread(NULL, NULL, ReceiveMessages, &clientSocketHandle, NULL, &threadID);
        Sleep(1);
    }

    closesocket(clientSocketHandle);
    WSACleanup();
    return 0;
}