#include <TrojanBlastServerPCH.h>
#include "Server.h"
#include "GameObjectRegistry.h"
#include "ShipServer.h"
#include "TommyCoinServer.h"

typedef GameObject* (*GameObjectCreationFunc)();

bool Server::StaticInit()
{
	sInstance.reset( new Server() );

	GameObjectRegistry::sInstance->RegisterCreationFunction('SHIP', ShipServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('COIN', TommyCoinServer::StaticCreate);

	return true;
}
