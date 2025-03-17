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
		return -1;
	}

	LPHOSTENT hostEnt;
	hostEnt = gethostbyname("localhost");
	if (!hostEnt)
	{
		std::cout << "Unable to get host name\n";
		WSACleanup();
		return SOCKET_ERROR;
	}
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket == INVALID_SOCKET)
	{
		std::cout << "Unable to create socket\n";
		WSACleanup();
		return SOCKET_ERROR;
	}

	std::string server_ip;
	std::cout << "IP to connect to: ";
	std::cin >> server_ip;
	std::cout << "\n";


	SOCKADDR_IN server_addr;
	server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	server_addr.sin_port = htons(2008);
	server_addr.sin_family = AF_INET;

	int return_value = connect(client_socket, (LPSOCKADDR)&server_addr, sizeof(server_addr));
	if (return_value == SOCKET_ERROR)
	{
		std::cout << "Unable to connect\n";
		WSACleanup();
		return SOCKET_ERROR;
	}
	std::cout << "Connected to server\n";

	std::string message_to_send;
	std::cout << "Input a message: ";
	std::cin.ignore();
	std::getline(std::cin, message_to_send);
	std::cout << "\nMessage input: " << message_to_send << "\n";

	return_value = send(client_socket, message_to_send.c_str(), message_to_send.size(), 0);
	if (return_value == SOCKET_ERROR)
	{
		std::cout << "Unable to send\n";
		WSACleanup();
		return SOCKET_ERROR;
	}
	closesocket(client_socket);
	WSACleanup();
}
