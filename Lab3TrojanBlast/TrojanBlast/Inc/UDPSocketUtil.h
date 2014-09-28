//#include "UDPSocket.h"

class UDPSocket;

class UDPSocketUtil
{
public:
	static shared_ptr<UDPSocket> Create(uint16_t inPortNum);
	static int StartUp();
	static void CleanUp();
	static int GetLastError();
};

