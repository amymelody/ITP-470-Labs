#include "stdafx.h"

void Client::Run(ULONG ipAddress, USHORT inPort, wstring name)
{
	WindowsConsole* wc = WindowsConsole::Instance();
	wc->FlushInputBuffer();
	wc->Write(name.c_str());
	wc->Write( L" Starting As Client!\n" );

	shared_ptr<TCPSocket> socket = TCPSocketUtil::CreateSocket(0);
	if (socket == NULL) {
		return;
	}

	wc->Write(L"Connecting to server...\n");
	if (socket->Connect(ipAddress, inPort) == SOCKET_ERROR) {
		LOG(L"Error Connecting: %d", GetLastError());
		return;
	}

	string nameStr(name.begin(), name.end());
	if (socket->Send(nameStr.c_str(), nameStr.length()) == SOCKET_ERROR) {
		LOG(L"Error Sending Name: %d", GetLastError());
		return;
	}

	if (socket->SetNonBlocking(true) == SOCKET_ERROR) {
		LOG(L"Error Setting NonBlocking: %d", GetLastError());
		return;
	}
	wc->Write(L"Connected\n");

	for (;;)
	{
		char data[64];
		int size = socket->Receive(data, sizeof(data));
		if (size != SOCKET_ERROR) {
			string dataStr(data);
			dataStr.resize(size);
			std::wstringstream wData;
			wData << dataStr.c_str();
			wc->Write(wData.str().c_str());
		}

		wc->ProcessPendingInput();
		std::wstring inputData;
		if (wc->ReadPendingLine( inputData ))
		{
			if (inputData.size() > 0)
			{
				wstring outputData = L"Me: " + inputData;
				wc->Write(outputData.c_str());

				char dest[64];
				size_t dataSize;
				wcstombs_s(&dataSize, dest, inputData.c_str(), sizeof(inputData));
				string inputStr(dest);

				if (socket->Send(inputStr.c_str(), inputStr.length()) == SOCKET_ERROR) {
					LOG(L"Error Sending: %d", GetLastError());
				}
			}

		}
	}
}