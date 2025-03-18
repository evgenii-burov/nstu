#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

const int PORT = 2004;
const int MAX_CLIENTS = 5;

SOCKET serverSocket;
std::vector<SOCKET> clientSockets;
std::mutex mtx; // Mutex for thread-safe access to shared resources

void HandleClient(SOCKET clientSocket) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safe console output
            std::cout << "Client disconnected." << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0'; // Null-terminate the received message

        // Lock for thread-safe console output
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Received from client: " << buffer << std::endl;
        }
    }

    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, MAX_CLIENTS) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed." << std::endl;
            continue;
        }

        // Lock for thread-safe access to clientSockets
        {
            std::lock_guard<std::mutex> lock(mtx);
            clientSockets.push_back(clientSocket);
            std::cout << "New client connected. Total clients: " << clientSockets.size() << std::endl;
        }

        // Create a new thread to handle the client
        std::thread clientThread(HandleClient, clientSocket);
        clientThread.detach(); // Detach the thread to let it run independently
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}