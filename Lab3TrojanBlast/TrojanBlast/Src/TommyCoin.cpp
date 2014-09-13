#include <TrojanBlastPCH.h>
#include "TommyCoin.h"
#include "Ship.h"
#include "ScoreBoardManager.h"

TommyCoin::TommyCoin()
{
	SetScale( GetScale() * 0.5f );
	SetCollisionRadius( 0.25f );
}


bool TommyCoin::HandleCollisionWithShip( Ship* inShip )
{
	//kill yourself!
	SetDoesWantToDie( true );

	ScoreBoardManager::sInstance->IncScore( inShip->GetPlayerID(), 1 );

	return false;
}
