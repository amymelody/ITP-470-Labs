#include <TrojanBlastServerPCH.h>


TommyCoinServer::TommyCoinServer()
{
	SetNetworkId( NetworkManagerServer::sInstance->GetNewNetworkId() );
}

