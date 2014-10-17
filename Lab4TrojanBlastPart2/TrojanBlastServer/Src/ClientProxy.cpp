#include <TrojanBlastServerPCH.h>


ClientProxy::ClientProxy( const SocketAddress& inSocketAddress, const wstring& inName, int inPlayerId ) :
mSocketAddress( inSocketAddress ),
mName( inName ),
mPlayerId( inPlayerId ),
mDeliveryNotificationManager( false, true )
{
	UpdateLastPacketTime();
}


void ClientProxy::UpdateLastPacketTime()
{
	mLastPacketFromClientTime = Timing::sInstance.GetTimef(); 
}

