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