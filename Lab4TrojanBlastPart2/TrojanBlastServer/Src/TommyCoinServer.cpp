#include <TrojanBlastServerPCH.h>


TommyCoinServer::TommyCoinServer()
{
}

void TommyCoinServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}



