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

#pragma comment(lib, "Ws2_32.lib")

typedef GameObject* (*GameObjectCreationFunc)();

bool Client::StaticInit(HINSTANCE hInstance, int inCmdShow, USHORT inPort, ULONG ipAddress, wstring name)
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
	wstring helo(mName);
	helo = L"HELO" + helo + L'\0';
	mNameBuffer->WriteData(helo.c_str(), sizeof(helo));

	return Engine::Run();
}

void Client::DoFrame() {
	Engine::DoFrame();
	RenderManager::sInstance->Render();

	if (timeElapsed >= 1) {
		timeElapsed = 0;
		if (mSocket->SendTo(mNameBuffer, *(reinterpret_cast<sockaddr*>(&mSockAddress))) == SOCKET_ERROR) {
			LOG(L"Error Sending: %d", GetLastError());
		}
	}
}