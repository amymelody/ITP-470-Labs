// TrojanChat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, WCHAR* argv[])
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	TCPSocketUtil::StartUp();

	if (argc == 2) {
		wstring wstr = argv[1];
		string str(wstr.begin(), wstr.end());
		USHORT inPort = atoi(str.c_str());

		Server server;
		server.Run(htons(inPort));
	} else if (argc > 2) {
		wstring wstr = argv[1];
		string portStr(wstr.begin(), wstr.end());

		for (unsigned int i = 0; i < wstr.length(); i++) {
			if (wstr[i] == ':') {
				portStr.erase(0, i+1);
				wstr.erase(i, portStr.length());
				break;
			}
		}

		ULONG ipAddress;
		InetPton(AF_INET, wstr.c_str(), &ipAddress);
		USHORT inPort = atoi(portStr.c_str());

		Client client;
		client.Run(ipAddress, htons(inPort), argv[2]);
	}

	TCPSocketUtil::CleanUp();

	return 0;
}

