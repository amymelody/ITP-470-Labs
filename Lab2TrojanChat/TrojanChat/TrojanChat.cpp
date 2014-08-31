// TrojanChat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, WCHAR* argv[])
{
	//Fill update main() so that it uses your logic to properly create
	//a client or server. Feel free to add a constructor to both classes
	//to take in any required parameters.

	if (argv[2] == NULL) {
		wstring wstr = argv[1];
		string str(wstr.begin(), wstr.end());
		uint16_t inPort = atoi(str.c_str());

		Server server;
		server.Run(inPort);
	} else {
		wstring wstr = argv[1];
		string str(wstr.begin(), wstr.end());
		ULONG ipAddress = atol(str.c_str());

		Client client;
		client.Run(ipAddress);
	}

	return 0;
}

