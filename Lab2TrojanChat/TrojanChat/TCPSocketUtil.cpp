#include "stdafx.h"

shared_ptr<TCPSocket> TCPSocketUtil::CreateSocket(USHORT inPort)
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		LOG(L"Error Invalid Socket: %d", GetLastError());
		return shared_ptr<TCPSocket>();
	}

	if (inPort != 0) {
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.S_un.S_addr = INADDR_ANY;
		address.sin_port = inPort;
		
		int bindResult = bind(s, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr_in));

		if (bindResult == SOCKET_ERROR) {
			LOG(L"Error Binding: %d", GetLastError());
			return shared_ptr<TCPSocket>();
		}
	}

	return shared_ptr<TCPSocket>(new TCPSocket(s));
}

int TCPSocketUtil::StartUp()
{
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void TCPSocketUtil::CleanUp()
{
	WSACleanup();
}

int TCPSocketUtil::GetLastError()
{
	return WSAGetLastError();
}

int TCPSocketUtil::Select(fd_set* readfds, fd_set* writefds, fd_set* exceptfds) {
	return select(0, readfds, writefds, exceptfds, NULL);
}