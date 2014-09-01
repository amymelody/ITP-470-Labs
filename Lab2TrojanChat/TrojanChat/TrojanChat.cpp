// TrojanChat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, WCHAR* argv[])
{
	TCPSocketUtil::StartUp();

	if (argc == 2) {
		wstring wstr = argv[1];
		string str(wstr.begin(), wstr.end());
		USHORT inPort = atoi(str.c_str());

		Server server;
		server.Run(htons(inPort));
	} else if (argc > 2) {
		wstring wstr = argv[1];

		ULONG ipAddress;
		InetPton(AF_INET, wstr.c_str(), &ipAddress);

		Client client;
		client.Run(ipAddress, htons(45000));
	}

	TCPSocketUtil::CleanUp();

	return 0;
}

