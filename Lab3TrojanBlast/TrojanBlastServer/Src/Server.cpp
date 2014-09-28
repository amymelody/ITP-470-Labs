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

	mPlayerCount = 0;

	return Engine::Run();
}

void Server::DoFrame() {
	Engine::DoFrame();

	PacketBuffer* outBuffer = new PacketBuffer();
	sockaddr outAddr;
	int retVal = mSocket->ReceiveFrom(outBuffer, outAddr);
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

			wstring wlcm;

			ClientProxy* cp = GetExistingClientProxy(outAddr);
			if (cp) {
				wlcm = std::to_wstring(cp->mPlayerID);
			}
			else {
				mPlayerCount++;
				msgStr.erase(0, 4);
				OutputDebugStringW(msgStr.c_str());
				mClientProxies.push_back(new ClientProxy(msgStr, mPlayerCount, outAddr));

				wlcm = std::to_wstring(mPlayerCount);
			}

			wlcm = L"WLCM" + wlcm;
			PacketBuffer* wlcmBuffer = new PacketBuffer();
			wlcmBuffer->WriteData(wlcm.c_str(), sizeof(wlcm));

			if (mSocket->SendTo(wlcmBuffer, outAddr) == SOCKET_ERROR) {
				LOG(L"Error Sending: %d", GetLastError());
			}
		}
	}
}

ClientProxy* Server::GetExistingClientProxy(sockaddr address) {
	for (ClientProxy* cp : mClientProxies) {
		sockaddr_in* cpAddress = reinterpret_cast<sockaddr_in*>(&cp->mAddress);
		sockaddr_in* testAddress = reinterpret_cast<sockaddr_in*>(&address);

		if (cpAddress->sin_port == testAddress->sin_port &&
			cpAddress->sin_addr.S_un.S_addr == testAddress->sin_addr.S_un.S_addr) {
			return cp;
		}
	}
	return NULL;
}