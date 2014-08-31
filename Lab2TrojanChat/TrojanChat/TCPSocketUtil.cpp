#include "stdafx.h"

TCPSocketPtr TCPSocketUtil::CreateSocket(uint16_t inPort)
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		return TCPSocketPtr();
	}

	if (inPort != 0) {
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.S_un.S_addr = INADDR_ANY;
		address.sin_port = htons(inPort);
		
		int bindResult = bind(s, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr));

		if (bindResult == SOCKET_ERROR) {
			return TCPSocketPtr();
		}
	}

	return TCPSocketPtr(new TCPSocket(s));
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

/*void TCPSocketUtil::ReportError(const wchar_t* inOperationDesc)
{
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	LOG(L"Error %ls: %d- %ls", inOperationDesc, errorNum, lpMsgBuf);
}*/