#include "stdafx.h"

TCPSocket::~TCPSocket()
{
	closesocket(mSocket);
}
