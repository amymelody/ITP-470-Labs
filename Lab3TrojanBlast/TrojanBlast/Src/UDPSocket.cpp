#include "TrojanBlastPCH.h"
#include "UDPSocket.h"

UDPSocket::~UDPSocket()
{
	closesocket(mSocket);
}

int UDPSocket::SendTo(const void* inData, uint32_t inLength, const sockaddr& inToAddress) {
	return sendto(mSocket, static_cast<const char*>(inData), inLength, 0, &inToAddress, sizeof(sockaddr));
}

int UDPSocket::ReceiveFrom(void* outData, uint32_t outLength, sockaddr& outFromAddress) {
	int addressSize = sizeof(outFromAddress);
	return recvfrom(mSocket, static_cast<char*>(outData), outLength, 0, &outFromAddress, &addressSize);
}

int UDPSocket::SetNonBlocking(bool inShouldBlock) {
	ULONG argp = inShouldBlock;
	return ioctlsocket(mSocket, FIONBIO, &argp);
}

int UDPSocket::SendTo(const PacketBuffer* inPacketBuffer, const sockaddr& inToAddress) {
	return sendto(mSocket, reinterpret_cast<const char*>(inPacketBuffer->mBuffer), inPacketBuffer->mBufferHead, 
		0, &inToAddress, sizeof(sockaddr));
}

int UDPSocket::ReceiveFrom(PacketBuffer* outPacketBuffer, sockaddr& outFromAddress) {
	int addressSize = sizeof(outFromAddress);
	int retVal = recvfrom(mSocket, reinterpret_cast<char*>(outPacketBuffer->mBuffer), outPacketBuffer->mBufferCapacity,
		0, &outFromAddress, &addressSize);
	if (retVal >= 0) {
		outPacketBuffer->mBufferHead = 0;
	}
	return retVal;
}