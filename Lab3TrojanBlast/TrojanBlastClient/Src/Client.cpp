#include "TrojanBlastClientPCH.h"
#include "Client.h"
#include "WindowManager.h"
#include "GraphicsDriver.h"
#include "TextureManager.h"
#include "RenderManager.h"
#include "HUD.h"
#include "InputManager.h"
#include "GameObjectRegistry.h"
#include "ShipClient.h"
#include "TommyCoinClient.h"
#include "UDPSocketUtil.h"
#include "UDPSocket.h"
#include "StringUtils.h"
#include "World.h"

#pragma comment(lib, "Ws2_32.lib")

typedef GameObject* (*GameObjectCreationFunc)();
std::unique_ptr< Client >	Client::sInstance;

bool Client::StaticInit(HINSTANCE hInstance, int inCmdShow, USHORT inPort, ULONG ipAddress, string name)
{

	if( FAILED( WindowManager::StaticInit( hInstance, inCmdShow ) ) )
	{
        return false;
	}

    if( FAILED( GraphicsDriver::StaticInit( WindowManager::sInstance->GetMainWindow() ) ) )
	{
        return false;
    }

	sInstance.reset( new Client(inPort, ipAddress, name) );

	GameObjectRegistry::sInstance->RegisterCreationFunction('SHIP', ShipClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('COIN', TommyCoinClient::StaticCreate);

	TextureManager::StaticInit();
	RenderManager::StaticInit();
	HUD::StaticInit();
	InputManager::StaticInit();

	return true;
}

namespace {
	GameObjectPtr findObjectWithID(uint32_t networkID) {
		for (int i = 0; i < World::sInstance->GetGameObjects().size(); i++) {
			GameObjectPtr obj = World::sInstance->GetGameObjects().at(i);
			if (obj->mNetworkID == networkID) {
				return obj;
			}
		}

		return NULL;
	}
}

int Client::Run() {

	mSocket = UDPSocketUtil::Create(0);
	if (mSocket == NULL) {
		return -1;
	}

	if (mSocket->SetNonBlocking(true) == SOCKET_ERROR) {
		LOG(L"Error Setting NonBlocking: %d", GetLastError());
		return -1;
	}

	mSockAddress.sin_family = AF_INET;
	mSockAddress.sin_addr.S_un.S_addr = mIpAddress;
	mSockAddress.sin_port = mInPort;

	mNameBuffer = new PacketBuffer();
	string helo(mName);
	helo = "HELO" + helo + '\0';
	mNameBuffer->WriteString(helo);

	mPlayerID = -1;

	return Engine::Run();
}

void Client::DoFrame() {
	Engine::DoFrame();
	RenderManager::sInstance->Render();

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
		string msgStr;
		outBuffer->ReadString(msgStr);
		string wlcmStr(msgStr);
		wlcmStr.erase(4);
		if (wlcmStr == "WLCM") {
			msgStr.erase(0, 4);
			mPlayerID = std::stoi(msgStr);
		}
		if (wlcmStr == "TJBS") {
			uint32_t networkID;
			outBuffer->ReadInt(networkID);
			while (networkID != -1) {
				GameObjectPtr obj = findObjectWithID(networkID);
				uint32_t ccCode;
				outBuffer->ReadInt(ccCode);
				if (!obj) {
					obj = GameObjectRegistry::sInstance->CreateGameObject(ccCode);
					obj->mNetworkID = networkID;
				}
				obj->Read(outBuffer);
				obj->updated = true;

				outBuffer->ReadInt(networkID);
			}

			for (int i = World::sInstance->GetGameObjects().size()-1; i >= 0; i--) {
				GameObjectPtr obj = World::sInstance->GetGameObjects().at(i);
				if (!obj->updated) {
					obj->SetDoesWantToDie(true);
				}
				else {
					obj->updated = false;
				}
			}
		}
	}

	if (mPlayerID == -1 && timeElapsed >= 1) {
		timeElapsed = 0;
		if (mSocket->SendTo(mNameBuffer, *(reinterpret_cast<sockaddr*>(&mSockAddress))) == SOCKET_ERROR) {
			LOG(L"Error Sending: %d", GetLastError());
		}
	}
	else if (mPlayerID != -1 && timeElapsed >= 1.0f / 30.0f) {
		timeElapsed = 0;
		PacketBuffer* dataBuffer = new PacketBuffer();
		
		string dataStr = "TJBC";
		dataBuffer->WriteString(dataStr);

		InputManager::sInstance->GetState().Write(dataBuffer);

		if (mSocket->SendTo(dataBuffer, *(reinterpret_cast<sockaddr*>(&mSockAddress))) == SOCKET_ERROR) {
			LOG(L"Error Sending: %d", GetLastError());
		}
	}
}