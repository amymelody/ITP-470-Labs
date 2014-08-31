#include "stdafx.h"

void Server::Run(uint16_t inPort)
{
	TCPSocketUtil::CreateSocket(inPort);
}