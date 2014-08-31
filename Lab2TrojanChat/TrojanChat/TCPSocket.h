#include "stdafx.h"

class TCPSocket
{
public:
	~TCPSocket();
	friend class TCPSocketUtil;

private:
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {};
	SOCKET mSocket;
};
typedef shared_ptr<TCPSocket> TCPSocketPtr;