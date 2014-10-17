
#include <TrojanBlastClientPCH.h>

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
	InputManager::StaticInit();

	HUD::StaticInit();

	sInstance.reset( new Client() );

	return true;
}

Client::Client()
{
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'SHIP', ShipClient::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'COIN', TommyCoinClient::StaticCreate );


	wstring destination = StringUtils::GetCommandLineArg( 1 );
	wstring name = StringUtils::GetCommandLineArg( 2 );

	SocketAddress serverAddress( destination );

	NetworkManagerClient::StaticInit( serverAddress, name );

	NetworkManagerClient::sInstance->SetDropPacketChance( 0.1f );
}



void Client::DoFrame()
{
	NetworkManagerClient::sInstance->ProcessIncomingPackets();

	Engine::DoFrame();

	RenderManager::sInstance->Render();

	NetworkManagerClient::sInstance->SendOutgoingPackets();
}