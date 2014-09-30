#pragma once
#include "InputState.h"

class ClientProxy
{
public:
	ClientProxy(string name, int playerID, int networkID, sockaddr address) : 
		mName(name), 
		mPlayerID(playerID), 
		mNetworkID(networkID),
		mAddress(address),
		timeSinceUpdate(0) {};
	~ClientProxy() {};

	sockaddr mAddress;
	int mPlayerID;
	int mNetworkID;
	InputState mInputState;
	float timeSinceUpdate;

private:
	string mName;
};

