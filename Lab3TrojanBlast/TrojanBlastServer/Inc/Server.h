#include "Engine.h"
#include "ClientProxy.h"

class Server : public Engine
{
public:

	static bool StaticInit(USHORT inPort);
	int Run();

private:
	Server(USHORT inPort) : mInPort(inPort), Engine() {};
	void DoFrame();
	ClientProxy* GetExistingClientProxy(sockaddr address);

	USHORT mInPort;
	vector<ClientProxy*> mClientProxies;
	int mPlayerCount;
};