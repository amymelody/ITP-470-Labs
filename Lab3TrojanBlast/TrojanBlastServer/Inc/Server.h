#include "Engine.h"

class Server : public Engine
{
public:

	static bool StaticInit(USHORT inPort);
	int Run();

private:
	Server(USHORT inPort) : mInPort(inPort), Engine() {};
	void DoFrame();
	USHORT mInPort;
};