#pragma once
#include "InputState.h"

class ClientProxy
{
public:
	ClientProxy(string name, int playerID, sockaddr address) : mName(name), mPlayerID(playerID), mAddress(address) {};
	~ClientProxy() {};

	sockaddr mAddress;
	int mPlayerID;
	InputState mInputState;

private:
	string mName;
};

