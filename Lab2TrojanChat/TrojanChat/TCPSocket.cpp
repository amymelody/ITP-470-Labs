#include "stdafx.h"

TCPSocket::~TCPSocket()
{
	closesocket(mSocket);
}

int TCPSocket::Listen() {
	return listen(mSocket, SOMAXCONN);
}

int TCPSocket::Connect(ULONG ipAddress, USHORT inPort) {
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.S_un.S_addr = ipAddress;
	address.sin_port = inPort;
	return connect(mSocket, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr_in));
}

shared_ptr<TCPSocket> TCPSocket::Accept() {
	sockaddr_in address;
	int i = sizeof(sockaddr_in);
	clientSocket = accept(mSocket, reinterpret_cast<sockaddr*>(&address), &i);
	return shared_ptr<TCPSocket>(new TCPSocket(clientSocket));
}

int TCPSocket::Send(const char* buf, int len, int flags) {
	return send(mSocket, buf, len, flags);
}

int TCPSocket::Receive(char* buf, int len, int flags) {
	return recv(mSocket, buf, len, flags);
}

int TCPSocket::SetNonBlocking(bool inShouldBlock) {
	ULONG argp = inShouldBlock;
	return ioctlsocket(mSocket, FIONBIO, &argp);
}

SOCKET TCPSocket::GetSocket() {
	return mSocket;
}