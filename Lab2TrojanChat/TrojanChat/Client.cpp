#include "stdafx.h"

void Client::Run(ULONG ipAddress, USHORT inPort)
{
	WindowsConsole* wc = WindowsConsole::Instance();
	wc->FlushInputBuffer();

	wc->Write( L"Starting As Client!\n" );

	shared_ptr<TCPSocket> socket = TCPSocketUtil::CreateSocket(0);
	if (socket != NULL) {
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.S_un.S_addr = ipAddress;
		address.sin_port = inPort;

		if (socket->Connect(reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr_in)) == SOCKET_ERROR) {
			LOG(L"Error Connecting: %d", GetLastError());
			return;
		}
		
	}

	char data[64];
	string dataStr(data);
	int size = socket->Receive(data, sizeof(data), 0);
	if (size == SOCKET_ERROR) {
		LOG(L"Error Receiving: %d", GetLastError());
	}
	else {
		string dataStr(data);
		dataStr.resize(size);
		std::wstringstream wData;
		wData << dataStr.c_str();

		wc->Write(wData.str().c_str());
	}

	for (;;)
	{
		

		/*
		wc->ProcessPendingInput();
		std::wstring data;
		if (wc->ReadPendingLine( data ))
		{

			//do whatever you want with the data- for instance, this will write it to the console...
			if (data.size() > 0)
			{

				WindowsConsole::Instance()->Write( data.c_str() );
			}


		}*/
	}
}