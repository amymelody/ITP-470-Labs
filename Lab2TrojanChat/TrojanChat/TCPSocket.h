#include "stdafx.h"

class TCPSocket
{
public:
	~TCPSocket();
	friend class TCPSocketUtil;
	int Listen();
	int Connect(const sockaddr *name, int namelen);

private:
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {};
	SOCKET mSocket;
};
typedef shared_ptr<TCPSocket> TCPSocketPtr;