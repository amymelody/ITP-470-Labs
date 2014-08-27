// TrojanChat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, WCHAR* argv[])
{
	
	//check your arguments to determin if you're a client or a server...
	//for now, we'll assume you are a client
	bool isClient = true;
	
	if (isClient)
	{
		Client client;

		client.Run();
	}
	else
	{
		Server server;

		server.Run();
	}



	return 0;
}

