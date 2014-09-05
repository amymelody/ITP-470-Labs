#include "stdafx.h"

ClientProxy::~ClientProxy()
{
}

shared_ptr<TCPSocket> ClientProxy::GetTCPSocket() {
	return clientSocket;
}
