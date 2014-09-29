#include <TrojanBlastServerPCH.h>
#include "TommyCoinServer.h"
#include "ScoreBoardManager.h"
#include "Ship.h"

bool TommyCoinServer::HandleCollisionWithShip(Ship* inShip)
{
	//kill yourself!
	SetDoesWantToDie(true);

	ScoreBoardManager::sInstance->IncScore(inShip->GetPlayerID(), 1);

	return false;
}