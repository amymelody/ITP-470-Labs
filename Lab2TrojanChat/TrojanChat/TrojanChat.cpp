// TrojanChat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, WCHAR* argv[])
{
	//Fill update main() so that it uses your logic to properly create
	//a client or server. Feel free to add a constructor to both classes
	//to take in any required parameters.

	if (argv[2] == NULL) {
		Server server;
		uint16_t inPort = static_cast<uint16_t>(*argv[1]);
		server.Run(inPort);
	} else {
		Client client;

		client.Run();
	}

	return 0;
}

