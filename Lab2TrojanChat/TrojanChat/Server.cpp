#include "stdafx.h"

void Server::Run(uint16_t inPort)
{
	WindowsConsole* wc = WindowsConsole::Instance();
	wc->FlushInputBuffer();

	wc->Write(L"Starting As Server!\n");

	TCPSocketPtr socket = TCPSocketUtil::CreateSocket(inPort);
	if (socket != NULL) {
		socket->Listen();
	}
}