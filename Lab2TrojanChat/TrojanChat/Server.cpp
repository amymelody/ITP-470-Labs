#include "stdafx.h"

void Server::Run(USHORT inPort)
{
	WindowsConsole* wc = WindowsConsole::Instance();
	wc->FlushInputBuffer();

	wc->Write(L"Starting As Server!\n");

	shared_ptr<TCPSocket> socket = TCPSocketUtil::CreateSocket(inPort);
	if (socket != NULL) {
		if (socket->Listen() == SOCKET_ERROR) {
			LOG(L"Error Listening: %d", GetLastError());
			return;
		}
		shared_ptr<TCPSocket> clientSocket = socket->Accept();
		if (clientSocket) {
			string msg = "Hello World";
			if (clientSocket->Send(msg.c_str(), msg.length(), 0) == SOCKET_ERROR) {
				LOG(L"Error Sending: %d", GetLastError());
			}
		}
	}
}