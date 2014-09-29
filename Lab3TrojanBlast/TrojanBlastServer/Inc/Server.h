#include "Engine.h"
#include "ClientProxy.h"

class Server : public Engine
{
public:

	static bool StaticInit(USHORT inPort);
	static std::unique_ptr< Server > sInstance;
	int Run();
	vector<ClientProxy*> mClientProxies;

private:
	Server(USHORT inPort) : mInPort(inPort), Engine() {};
	void CreateRandomCoins(int inCoinCount);
	void SetupWorld();
	void DoFrame();
	ClientProxy* GetExistingClientProxy(sockaddr address);

	USHORT mInPort;
	uint32_t mPlayerCount;
	uint32_t mObjectCount;
};