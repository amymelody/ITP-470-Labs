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

typedef GameObject* (*GameObjectCreationFunc)();

bool Client::StaticInit( HINSTANCE hInstance, int inCmdShow )
{
	if( FAILED( WindowManager::StaticInit( hInstance, inCmdShow ) ) )
	{
        return false;
	}

    if( FAILED( GraphicsDriver::StaticInit( WindowManager::sInstance->GetMainWindow() ) ) )
	{
        return false;
    }

	TextureManager::StaticInit();
	RenderManager::StaticInit();
	HUD::StaticInit();
	InputManager::StaticInit();

	sInstance.reset( new Client() );

	GameObjectRegistry::sInstance->RegisterCreationFunction('SHIP', ShipClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('COIN', TommyCoinClient::StaticCreate);

	return true;
}

void Client::DoFrame() {
	RenderManager::sInstance->Render();
}