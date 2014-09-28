#include "PacketBuffer.h"

class UDPSocket
{
public:
	~UDPSocket();
	friend class UDPSocketUtil;
	int SendTo(const void* inData, uint32_t inLength, const sockaddr& inToAddress);
	int SendTo(const PacketBuffer* inPacketBuffer, const sockaddr& inToAddress);
	int ReceiveFrom(void* outData, uint32_t outLength, sockaddr& outFromAddress);
	int ReceiveFrom(PacketBuffer* outPacketBuffer, sockaddr& outFromAddress);
	int SetNonBlocking(bool inShouldBlock);

private:
	UDPSocket(SOCKET inSocket) : mSocket(inSocket) {};
	SOCKET mSocket;
};

