#include "Engine.h"
#include "PacketBuffer.h"

class Client : public Engine
{
public:
	static bool StaticInit( HINSTANCE hInstance, int inCmdShow, USHORT inPort, ULONG ipAddress, wstring name );
	int Run();

private:
	Client(USHORT inPort, ULONG ipAddress, wstring name) : mInPort(inPort), mIpAddress(ipAddress), mName(name), Engine() {};
	void DoFrame();
	wstring mName;
	USHORT mInPort;
	ULONG mIpAddress;
	sockaddr_in mSockAddress;
	PacketBuffer* mNameBuffer;
};