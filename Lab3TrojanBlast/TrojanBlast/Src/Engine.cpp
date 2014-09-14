#include <TrojanBlastPCH.h>
#include "Engine.h"
#include <time.h>
#include "GameObjectRegistry.h"
#include "World.h"
#include "ScoreBoardManager.h"
#include "Ship.h"
#include "StringUtils.h"
#include "MathUtils.h"
#include "Timing.h"

std::unique_ptr< Engine >	Engine::sInstance;



Engine::Engine()
{
	srand( static_cast< uint32_t >( time( nullptr ) ) );
	
	GameObjectRegistry::StaticInit();
	World::StaticInit();
	ScoreBoardManager::StaticInit();
}

namespace
{

	void CreateRandomCoins(int inCoinCount)
	{
		XMVECTOR coinMin = XMVectorSet(-5.f, -3.f, 0.f, 0.f);
		XMVECTOR coinMax = XMVectorSet(5.f, 3.f, 0.f, 0.f);
		GameObjectPtr go;

		//make a coin somewhere- where will these come from?
		for (int i = 0; i < inCoinCount; ++i)
		{
			go = GameObjectRegistry::sInstance->CreateGameObject('COIN');
			XMVECTOR coinLocation = MathUtils::GetRandomVector(coinMin, coinMax);
			go->SetLocation(coinLocation);
		}
	}

	ShipPtr CreateShipForPlayer(uint32_t inPlayerID)
	{
		ShipPtr ship;
		ship = std::static_pointer_cast< Ship >(GameObjectRegistry::sInstance->CreateGameObject('SHIP'));
		ship->SetColor(ScoreBoardManager::sInstance->GetEntry(inPlayerID)->GetColor());
		ship->SetPlayerID(inPlayerID);

		return ship;
	}
}

void Engine::SetupWorld()
{
	//add player on the scoreboard...
	wstring playerName = StringUtils::GetCommandLineArg( 1 );
	if( playerName.size() <= 0 )
	{
		playerName = L"No Name";
	}
	ScoreBoardManager::sInstance->AddEntry( 1, playerName );

	//spawn a ship and make it locally controlled...
	ShipPtr playerShip = CreateShipForPlayer(1);
	playerShip->SetIsLocallyControlled(true);

	//spawn some random coins
	CreateRandomCoins(10);

	//add a dummy ship
	ScoreBoardManager::sInstance->AddEntry(2, L"Dummy");
	ShipPtr dummyShip = CreateShipForPlayer(2);
	dummyShip->SetLocation(XMVectorSet(-2.f, 0.f, 0.f, 0.f));

	//spawn more random coins!
	CreateRandomCoins(10);
	
}

int Engine::Run()
{
	SetupWorld();

	return DoRunLoop();
}

int Engine::DoRunLoop()
{
	// Main message loop
	MSG msg = {0};
	while( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			DoFrame();
		}
	}

	return ( int )msg.wParam;
}

void Engine::DoFrame()
{
  
	Timing::sInstance.Update();

	World::sInstance->Update();

}