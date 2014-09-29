#include "Engine.h"
#include "PacketBuffer.h"

class Client : public Engine
{
public:
	static bool StaticInit( HINSTANCE hInstance, int inCmdShow, USHORT inPort, ULONG ipAddress, string name );
	static std::unique_ptr< Client > sInstance;
	int Run();

private:
	Client(USHORT inPort, ULONG ipAddress, string name) : mInPort(inPort), mIpAddress(ipAddress), mName(name), Engine() {};
	void DoFrame();

	string mName;
	USHORT mInPort;
	ULONG mIpAddress;
	sockaddr_in mSockAddress;
	PacketBuffer* mNameBuffer;
	int mPlayerID;
};