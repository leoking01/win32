// Jardin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[])
{
	SOCKET mySock;
	WSADATA wsa;
	sockaddr_in server;
	char* data = NULL;
	char server_reply[2000];
	int recv_size;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cout << "Error initalizing winsock " << WSAGetLastError() << std::endl;
	}

	mySock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mySock == INVALID_SOCKET) {
		std::cout << "Error socket(): " << WSAGetLastError() << std::endl;
		return 1;
	}
	
	// convierte la ip a formato binario
	inet_pton(AF_INET, "172.217.168.163", &server.sin_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(80); 

	if (connect(mySock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		std::cout << "connect() error " << WSAGetLastError() << std::endl;
		return 1;
	}
	
	std::cout << "conectado" << std::endl;

	data = (char *)"GET / HTTP/1.1\r\n\r\n";
	if (send(mySock, data, strlen(data), 0) == SOCKET_ERROR) {
		std::cout << "send() error " << WSAGetLastError() << std::endl;
		return 1;
	} 

	std::cout << "data sent:" << std::endl;
	std::cout << data << std::endl;

	if ((recv_size = recv(mySock, server_reply, 2000, 0)) == SOCKET_ERROR) {
		std::cout << "recv() error " << WSAGetLastError() << std::endl;
		return 1;
	}
	server_reply[recv_size] = '\0';
	std::cout << "Server response:" << std::endl;
	std::cout << server_reply << std::endl;

	closesocket(mySock);
	WSACleanup();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
