#include <TrojanBlastServerPCH.h>
#include "Server.h"
#include "GameObjectRegistry.h"
#include "ShipServer.h"
#include "TommyCoinServer.h"
#include "UDPSocketUtil.h"
#include "UDPSocket.h"
#include "StringUtils.h"

#pragma comment(lib, "Ws2_32.lib")

typedef GameObject* (*GameObjectCreationFunc)();

bool Server::StaticInit(USHORT inPort)
{
	sInstance.reset( new Server(inPort) );

	GameObjectRegistry::sInstance->RegisterCreationFunction('SHIP', ShipServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('COIN', TommyCoinServer::StaticCreate);

	return true;
}

int Server::Run() {
	mSocket = UDPSocketUtil::Create(mInPort);
	if (mSocket == NULL) {
		return -1;
	}

	if (mSocket->SetNonBlocking(true) == SOCKET_ERROR) {
		LOG(L"Error Setting NonBlocking: %d", GetLastError());
		return -1;
	}

	return Engine::Run();
}

void Server::DoFrame() {
	Engine::DoFrame();

	//in the run loop, add a funciton that checks for a packet 
	//on the socket. if there is a packet, see if it begins with
	//HELO. If so, call OutputDebugStringW with the name of the
	//client in the packet

	PacketBuffer* outBuffer = new PacketBuffer();
	sockaddr_in outAddr;
	int retVal = mSocket->ReceiveFrom(outBuffer, *(reinterpret_cast<sockaddr*>(&outAddr)));
	if (retVal == SOCKET_ERROR) {
		int error = GetLastError();
		if (error != WSAEWOULDBLOCK) {
			LOG(L"Error Receiving: %d", error);
		}
	}
	if (retVal > 0) {
		wchar_t* message = new wchar_t[retVal];
		outBuffer->ReadData(message, retVal);
		wstring msgStr(message);
		wstring heloStr(message);
		heloStr.erase(4, heloStr.length() - 4);
		if (heloStr == L"HELO") {
			msgStr.erase(0, 4);
			OutputDebugStringW(msgStr.c_str());
		}
	}
}
