#include "stdafx.h"

TCPSocket::~TCPSocket()
{
	closesocket(mSocket);
}

int TCPSocket::Listen() {
	return listen(mSocket, SOMAXCONN);
}

int TCPSocket::Connect(const sockaddr *name, int namelen) {
	return connect(mSocket, name, namelen);
}