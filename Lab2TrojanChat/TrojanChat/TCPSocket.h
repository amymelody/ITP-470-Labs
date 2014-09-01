#include "stdafx.h"

class TCPSocket
{
public:
	~TCPSocket();
	friend class TCPSocketUtil;
	int Listen();
	int Connect(const sockaddr *name, int namelen);
	shared_ptr<TCPSocket> Accept();
	int Send(const char* buf, int len, int flags);
	int Receive(char* buf, int len, int flags);

private:
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {};
	SOCKET mSocket;
	SOCKET clientSocket;
};