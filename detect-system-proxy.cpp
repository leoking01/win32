#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <iostream>
#include <string>
#pragma comment(lib, "wininet.lib")

int main() {
	//Fetches Proxy Details
	DWORD dwSize;
	LPINTERNET_PROXY_INFO ProxyInfo;
	char lpszData[100] = "";
	std::string LocalProxyStr;
	std::string LocalProxyPortString;
	std::string LocalProxyString;
	int del;  
	char LocalProxy[50];
	int LocalProxyPort;

	InternetQueryOption(NULL, INTERNET_OPTION_PROXY, lpszData, &dwSize);
	ProxyInfo = (LPINTERNET_PROXY_INFO)lpszData;
	printf("Proxy Connection: %s\n", (ProxyInfo->lpszProxy));

	// split proxy ip and port from lpszProxy member
	LocalProxyStr = (char*)ProxyInfo->lpszProxy;
	del = LocalProxyStr.find(":"); 
	LocalProxyString = LocalProxyStr.substr(0, del);
	LocalProxyPortString = LocalProxyStr.substr(del + 1);
	LocalProxyPort = std::stoi(LocalProxyPortString);
	std::cout << "ip:" << LocalProxyString << " port:" << LocalProxyPort << std::endl;

	return 0;
}