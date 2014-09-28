#include "TrojanBlastPCH.h"
#include "UDPSocketUtil.h"
#include "StringUtils.h"
#include "UDPSocket.h"

shared_ptr<UDPSocket> UDPSocketUtil::Create(uint16_t inPortNum) {
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET) {
		LOG(L"Error Invalid Socket: %d", GetLastError());
		return shared_ptr<UDPSocket>();
	}

//	if (inPortNum != 0) {
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.S_un.S_addr = INADDR_ANY;
		address.sin_port = inPortNum;

		int bindResult = bind(s, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr_in));

		if (bindResult == SOCKET_ERROR) {
			LOG(L"Error Binding: %d", GetLastError());
			return shared_ptr<UDPSocket>();
		}
//	}

	return shared_ptr<UDPSocket>(new UDPSocket(s));
}

int UDPSocketUtil::StartUp()
{
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void UDPSocketUtil::CleanUp()
{
	WSACleanup();
}

int UDPSocketUtil::GetLastError()
{
	return WSAGetLastError();
}
