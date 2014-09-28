#pragma once
class ClientProxy
{
public:
	ClientProxy(wstring name, int playerID, sockaddr address) : mName(name), mPlayerID(playerID), mAddress(address) {};
	~ClientProxy() {};

	sockaddr mAddress;
	int mPlayerID;

private:
	wstring mName;
};

