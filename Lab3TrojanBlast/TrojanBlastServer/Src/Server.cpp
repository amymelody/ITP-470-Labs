#include <TrojanBlastServerPCH.h>
#include "Server.h"
#include "GameObjectRegistry.h"
#include "ShipServer.h"
#include "TommyCoinServer.h"
#include "UDPSocketUtil.h"
#include "UDPSocket.h"
#include "StringUtils.h"
#include "MathUtils.h"
#include "ScoreBoardManager.h"
#include "World.h"
#include "Timing.h"

#pragma comment(lib, "Ws2_32.lib")

typedef GameObject* (*GameObjectCreationFunc)();

std::unique_ptr< Server >	Server::sInstance;

bool Server::StaticInit(USHORT inPort)
{
	sInstance.reset( new Server(inPort) );

	GameObjectRegistry::sInstance->RegisterCreationFunction('SHIP', ShipServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('COIN', TommyCoinServer::StaticCreate);

	return true;
}


void Server::CreateRandomCoins(int inCoinCount)
{
	XMVECTOR coinMin = XMVectorSet(-5.f, -3.f, 0.f, 0.f);
	XMVECTOR coinMax = XMVectorSet(5.f, 3.f, 0.f, 0.f);
	GameObjectPtr go;

	//make a coin somewhere- where will these come from?
	for (int i = 0; i < inCoinCount; ++i)
	{
		mObjectCount++;
		go = GameObjectRegistry::sInstance->CreateGameObject('COIN');
		XMVECTOR coinLocation = MathUtils::GetRandomVector(coinMin, coinMax);
		go->SetLocation(coinLocation);
		go->mNetworkID = mObjectCount;
	}
}

namespace {

	ShipPtr CreateShipForPlayer(uint32_t inPlayerID, uint32_t inNetworkID)
	{
		ShipPtr ship;
		ship = std::static_pointer_cast<Ship>(GameObjectRegistry::sInstance->CreateGameObject('SHIP'));
		ship->SetColor(ScoreBoardManager::sInstance->GetEntry(inPlayerID)->GetColor());
		ship->SetPlayerID(inPlayerID);
		ship->mNetworkID = inNetworkID;

		return ship;
	}
}

void Server::SetupWorld()
{

	//spawn some random coins
	CreateRandomCoins(10);

	//spawn more random coins!
	CreateRandomCoins(10);

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
	mObjectCount = 0;

	SetupWorld();

	return Engine::Run();
}

namespace {
	GameObjectPtr findObjectWithID(uint32_t networkID) {
		for (unsigned int i = 0; i < World::sInstance->GetGameObjects().size(); i++) {
			GameObjectPtr obj = World::sInstance->GetGameObjects().at(i);
			if (obj->mNetworkID == networkID) {
				return obj;
			}
		}

		return NULL;
	}
}

void Server::DoFrame() {
	Engine::DoFrame();

	for (int i = mClientProxies.size() - 1; i >= 0; i--) {
		mClientProxies.at(i)->timeSinceUpdate += Timing::sInstance.GetDeltaTime();
		if (mClientProxies.at(i)->timeSinceUpdate >= 3.0f) {
			ScoreBoardManager::sInstance->RemoveEntry(mClientProxies.at(i)->mPlayerID);
			findObjectWithID(mClientProxies.at(i)->mNetworkID)->SetDoesWantToDie(true);
			mClientProxies.erase(mClientProxies.begin()+i);
		}
	}

	PacketBuffer* outBuffer = new PacketBuffer();
	sockaddr outAddr;
	int retVal = mSocket->ReceiveFrom(outBuffer, outAddr);
	if (retVal == SOCKET_ERROR) {
		int error = GetLastError();
		if (error != WSAEWOULDBLOCK && error != WSAECONNRESET) {
			LOG(L"Error Receiving: %d", error);
		}
	}
	if (retVal > 0) {
		string msgStr;
		outBuffer->ReadString(msgStr);
		string identifierStr(msgStr);
		identifierStr.erase(4);
		if (identifierStr == "HELO") {

			string wlcm;

			ClientProxy* cp = GetExistingClientProxy(outAddr);
			if (cp) {
				wlcm = std::to_string(cp->mPlayerID);
			}
			else {
				mPlayerCount++;
				mObjectCount++;
				msgStr.erase(0, 4);
				mClientProxies.push_back(new ClientProxy(msgStr, mPlayerCount, mObjectCount, outAddr));

				wlcm = std::to_string(mPlayerCount);

				wstring playerName(msgStr.begin(), msgStr.end());
				if (playerName.size() <= 0)
				{
					playerName = L"No Name";
				}
				ScoreBoardManager::sInstance->AddEntry(mPlayerCount, playerName);

				ShipPtr playerShip = CreateShipForPlayer(mPlayerCount, mObjectCount);
			}

			wlcm = "WLCM" + wlcm;
			PacketBuffer* wlcmBuffer = new PacketBuffer();
			wlcmBuffer->WriteString(wlcm);

			if (mSocket->SendTo(wlcmBuffer, outAddr) == SOCKET_ERROR) {
				LOG(L"Error Sending: %d", GetLastError());
			}
		}
		
		if (identifierStr == "TJBC") {

			ClientProxy* cp = GetExistingClientProxy(outAddr);
			if (cp) {
				cp->timeSinceUpdate = 0;
				cp->mInputState.Read(outBuffer);
			}

		}
		
	}

	//Send entire world state
	if (timeElapsed >= 1.0f / 30.0f) {
		timeElapsed = 0;
		PacketBuffer* dataBuffer = new PacketBuffer();

		string dataStr = "TJBS";
		dataBuffer->WriteString(dataStr);
		uint32_t numGameObjects = World::sInstance->GetGameObjects().size();
		dataBuffer->WriteInt(numGameObjects);

		for (int i = 0; i < numGameObjects; i++) {
			GameObjectPtr obj = World::sInstance->GetGameObjects().at(i);
			dataBuffer->WriteInt(obj->mNetworkID);
			uint32_t idName = obj->GetFourCCName();
			dataBuffer->WriteInt(idName);
			obj->Write(dataBuffer);
		}

		uint32_t numEntries = ScoreBoardManager::sInstance->GetEntries().size();
		dataBuffer->WriteInt(numEntries);

		for (int i = 0; i < numEntries; i++) {
			ScoreBoardManager::Entry entry = ScoreBoardManager::sInstance->GetEntries().at(i);
			uint32_t playerID = entry.GetPlayerID();
			dataBuffer->WriteInt(playerID);
			wstring playerName = entry.GetPlayerName();
			string name(playerName.begin(), playerName.end());
			dataBuffer->WriteString(name);
			entry.Write(dataBuffer);
		}

		for (ClientProxy* cp : mClientProxies) {
			if (mSocket->SendTo(dataBuffer, cp->mAddress) == SOCKET_ERROR) {
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