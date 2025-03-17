#pragma comment (lib,"Ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>
#include <iostream> 
#include <sstream>
#include <string>

int main()
{
	//Launching WSA
	WSADATA wsa_data;
	WORD winsock_version = MAKEWORD(2, 2);
	if (WSAStartup(winsock_version, &wsa_data) != 0)
	{
		std::cout << "WSAStartup failed\n";
		return SOCKET_ERROR;
	}
	//Setting servers address
	SOCKADDR_IN server_addr;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(2008);
	server_addr.sin_family = AF_INET;
	//Creating server socket
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == INVALID_SOCKET)
	{
		std::cout << "Unable to create socket\n";
		WSACleanup();
		return SOCKET_ERROR;
	}
	//Binding the socket
	int return_value = bind(server_socket, (LPSOCKADDR)&server_addr, sizeof(server_addr));
	if (return_value == SOCKET_ERROR)
	{
		std::cout << "Binding failed\n";
		WSACleanup();
		return SOCKET_ERROR;
	}
	std::cout << "Server started on address " << inet_ntoa(server_addr.sin_addr) << ", port " << htons(server_addr.sin_port) << "\n";

	while (true)
	{
		//Listening to the socket
		return_value = listen(server_socket, 10);
		if (return_value == SOCKET_ERROR)
		{
			std::cout << "Unable to listen\n";
			WSACleanup();
			return SOCKET_ERROR;
		}

		SOCKET client_socket;
		SOCKADDR_IN client_addr;
		int client_addr_size = sizeof(client_addr);
		client_socket = accept(server_socket, (LPSOCKADDR)&client_addr, &client_addr_size);
		if (client_socket == INVALID_SOCKET)
		{
			std::cout << "Unable to accept\n";
			WSACleanup();
			return SOCKET_ERROR;
		}
		std::cout << "Connection accepted from " << inet_ntoa(client_addr.sin_addr) \
			<< ", port " << htons(client_addr.sin_port) << "\n";

		char buffer[1024];
		return_value = recv(client_socket, buffer, sizeof(buffer), 0);
		if (return_value > 0 && return_value < 1024)
		{
			buffer[return_value] = '\0';
			std::cout << "Received message: " << buffer << "\n";
		}
		else if (return_value == 0)
		{
			std::cout << "Connection closed\n";
		}
		else
		{
			std::cout << "Unable to receive\n";
			WSACleanup();
			return SOCKET_ERROR;
		}
		std::cout << buffer << "\n";
		std::cout << strcmp(buffer, "shutdown") << "\n";
		if (!strcmp(buffer, "shutdown"))
		{
			std::cout << "Shutting the server down";
			closesocket(client_socket);
			WSACleanup();
			break;
		}
	}
}