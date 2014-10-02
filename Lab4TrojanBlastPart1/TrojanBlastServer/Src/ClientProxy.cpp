#include <TrojanBlastServerPCH.h>


ClientProxy::ClientProxy( const SocketAddress& inSocketAddress, const wstring& inName, int inPlayerId ) :
mSocketAddress( inSocketAddress ),
mName( inName ),
mPlayerId( inPlayerId ),
mDeliveryNotificationManager( false, true ),
mLastLogStatTime( 0.f )
{
	UpdateLastPacketTime();
}


void ClientProxy::UpdateLastPacketTime()
{
	float time = Timing::sInstance.GetTimef();

	mLastPacketFromClientTime = time;

	//and, if it's been more than 5 seconds since hte last time we dumped the stats...
	if (time > mLastLogStatTime + 5.f)
	{
		mDeliveryNotificationManager.LogStats( mName );
		mLastLogStatTime = time;
	}

}

