#include "stdafx.h"

void Client::Run(ULONG ipAddress)
{
	WindowsConsole* wc = WindowsConsole::Instance();
	wc->FlushInputBuffer();

	wc->Write( L"Starting As Client!\n" );

	TCPSocketPtr socket = TCPSocketUtil::CreateSocket(0);
	if (socket != NULL) {
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.S_un.S_addr = ipAddress;

		socket->Connect(reinterpret_cast<sockaddr*>(&address), sizeof(address));
	}

	for (;;)
	{
		wc->ProcessPendingInput();
		std::wstring data;
		if (wc->ReadPendingLine( data ))
		{

			//do whatever you want with the data- for instance, this will write it to the console...
			if (data.size() > 0)
			{

				WindowsConsole::Instance()->Write( data.c_str() );
			}


		}
	}
}