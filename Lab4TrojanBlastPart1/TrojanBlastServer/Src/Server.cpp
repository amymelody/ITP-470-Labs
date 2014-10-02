
#include <TrojanBlastServerPCH.h>



//uncomment this when you begin working on the server

bool Server::StaticInit()
{
	sInstance.reset( new Server() );

	return true;
}

Server::Server()
{

	GameObjectRegistry::sInstance->RegisterCreationFunction( 'SHIP', ShipServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'COIN', TommyCoinServer::StaticCreate );

	InitNetworkManager();
	
	//NetworkManagerServer::sInstance->SetDropPacketChance( 0.8f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.3f );

}


int Server::Run()
{
	GameObject* tcs = TommyCoinServer::StaticCreate();

	SetupWorld();

	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	wstring portString = StringUtils::GetCommandLineArg( 1 );
	uint16_t port = _wtoi( portString.c_str() );

	return NetworkManagerServer::StaticInit( port );
}


namespace
{
	
	void CreateRandomCoins( int inCoinCount )
	{
		XMVECTOR coinMin = XMVectorSet( -5.f, -3.f, 0.f, 0.f );
		XMVECTOR coinMax = XMVectorSet( 5.f, 3.f, 0.f, 0.f );
		GameObjectPtr go;

		//make a coin somewhere- where will these come from?
		for( int i = 0; i < inCoinCount; ++i )
		{
			go = GameObjectRegistry::sInstance->CreateGameObject( 'COIN' );
			XMVECTOR coinLocation = MathUtils::GetRandomVector( coinMin, coinMax );
			go->SetLocation( coinLocation );
		}
	}


}


void Server::SetupWorld()
{

	//spawn some random coins
	CreateRandomCoins( 10 );
	
	//spawn more random coins!
	CreateRandomCoins( 10 );
}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	Engine::DoFrame();

	NetworkManagerServer::sInstance->SendOutgoingPackets();

}

void Server::HandleNewClient( ClientProxyPtr inClientProxy )
{
	ShipPtr ship;
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->AddEntry( playerId, inClientProxy->GetName() );

	ship = std::static_pointer_cast< Ship >( GameObjectRegistry::sInstance->CreateGameObject( 'SHIP' ) );
	ship->SetColor( ScoreBoardManager::sInstance->GetEntry( playerId )->GetColor() );
	ship->SetPlayerId( playerId );
	//gotta pick a better spawn location than this...
	ship->SetLocation( XMVectorSet( static_cast< float >( -playerId ), 0.f, 0.f, 0.f ) );

}

void Server::HandleLostClient( ClientProxyPtr inClientProxy )
{
	//kill client's ship
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->RemoveEntry( playerId );
	ShipPtr ship = GetShipForPlayer( playerId );
	if( ship )
	{
		ship->SetDoesWantToDie( true );
	}
}

ShipPtr Server::GetShipForPlayer( int inPlayerId )
{
	//run through the objects till we find the ship...
	//it would be nice if we kept a pointer to the ship on the clientproxy
	//but then we'd have to clean it up when the ship died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for( int i = 0, c = gameObjects.size(); i < c; ++i )
	{
		GameObjectPtr go = gameObjects[ i ];
		Ship* ship = go->GetAsShip();
		if( ship && ship->GetPlayerId() == inPlayerId )
		{
			return std::static_pointer_cast< Ship >( go );
		}
	}

	return nullptr;

}