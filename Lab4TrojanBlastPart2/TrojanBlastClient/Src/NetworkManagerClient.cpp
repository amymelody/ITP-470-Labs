#include <TrojanBlastClientPCH.h>

NetworkManagerClient*	NetworkManagerClient::sInstance;

namespace
{
	const float kTimeBetweenHellos = 1.f;
	const float kTimeBetweenInputPackets = 0.033f;
}

NetworkManagerClient::NetworkManagerClient() :
	mState( NCS_Uninitialized ),
	mDeliveryNotificationManager( true, false )
{
}

void NetworkManagerClient::StaticInit( const SocketAddress& inServerAddress, const wstring& inName )
{
	sInstance = new NetworkManagerClient();
	return sInstance->Init( inServerAddress, inName );
}

void NetworkManagerClient::Init( const SocketAddress& inServerAddress, const wstring& inName )
{
	NetworkManager::Init( 0 );

	mServerAddress = inServerAddress;
	mState = NCS_SayingHello;
	mTimeOfLastHello = 0.f;
	mName = inName;
}

void NetworkManagerClient::ProcessPacket( IncomingPacketBuffer& inPacketBuffer, const SocketAddress& inFromAddress )
{
	uint32_t	packetType;
	if( inPacketBuffer.Read( packetType ) )
	{
		switch( packetType )
		{
		case kWelcomeCC:
			HandleWelcomePacket( inPacketBuffer );
			break;
		case kStateCC:
			if( mDeliveryNotificationManager.ReadAndProcessState( inPacketBuffer ) )
			{
				HandleStatePacket( inPacketBuffer );
			}
			break;
		}
	}
}


void NetworkManagerClient::SendOutgoingPackets()
{
	switch( mState )
	{
	case NCS_SayingHello:
		UpdateSayingHello();
		break;
	case NCS_Welcomed:
		UpdateSendingInputPacket();
		break;
	}
}

void NetworkManagerClient::UpdateSayingHello()
{
	float time = Timing::sInstance.GetTimef();

	if( time > mTimeOfLastHello + kTimeBetweenHellos )
	{
		SendHelloPacket();
		mTimeOfLastHello = time;
	}
}

void NetworkManagerClient::SendHelloPacket()
{
	OutgoingPacketBuffer helloPacket; 

	helloPacket.Write( kHelloCC );
	helloPacket.Write( mName );

	SendPacket( helloPacket, mServerAddress );
}

void NetworkManagerClient::HandleWelcomePacket( IncomingPacketBuffer& inPacketBuffer )
{
	//if we got a player id, we've been welcomed!
	int playerId;
	if( inPacketBuffer.Read( playerId ) )
	{
		mPlayerId = playerId;
		mState = NCS_Welcomed;
		LOG( L"'%ls' was welcomed on client as player %d", mName.c_str(), mPlayerId );
	}
}



void NetworkManagerClient::HandleStatePacket( IncomingPacketBuffer& inPacketBuffer )
{
	//old
	//HandleGameObjectState( inPacketBuffer );
	
	HandleScoreBoardState( inPacketBuffer );

	//tell the replication manager to handle the rest...
	mReplicationManagerClient.Read( inPacketBuffer );
}



void NetworkManagerClient::HandleGameObjectState( IncomingPacketBuffer& inPacketBuffer )
{
	//copy the mNetworkIdToGameObjectMap so that anything that doesn't get an updated can be destroyed...
	IntToGameObjectMap	objectsToDestroy = mNetworkIdToGameObjectMap;

	int stateCount;
	if( inPacketBuffer.Read( stateCount ) && stateCount > 0 )
	{
		for( int stateIndex = 0; stateIndex < stateCount; ++stateIndex )
		{
			int networkId;
			uint32_t fourCC;

			if( inPacketBuffer.Read( networkId ) )
			{
				if( inPacketBuffer.Read( fourCC ) )
				{
					GameObjectPtr go;
					auto itGO = mNetworkIdToGameObjectMap.find( networkId );
					//didn't find it, better create it!
					if( itGO == mNetworkIdToGameObjectMap.end() )
					{
						go = GameObjectRegistry::sInstance->CreateGameObject( fourCC );
						go->SetNetworkId( networkId );
						AddToNetworkIdToGameObjectMap( go );
					}
					else
					{
						//found it
						go = itGO->second;
					}

					//now we can update into it
					go->Read( inPacketBuffer );
					objectsToDestroy.erase( networkId );
				}
			}
		}
	}

	//anything left gets the axe
	DestroyGameObjectsInMap( objectsToDestroy );
}

void NetworkManagerClient::HandleScoreBoardState( IncomingPacketBuffer& inPacketBuffer )
{
	ScoreBoardManager::sInstance->Read( inPacketBuffer );
}
 
void NetworkManagerClient::DestroyGameObjectsInMap( const IntToGameObjectMap& inObjectsToDestroy )
{
	for( auto& pair: inObjectsToDestroy )
	{
		pair.second->SetDoesWantToDie( true );
		//and remove from our map!
		mNetworkIdToGameObjectMap.erase( pair.first );
	}

	
}


void NetworkManagerClient::UpdateSendingInputPacket()
{
	float time = Timing::sInstance.GetTimef();

	if( time > mTimeOfLastInputPacket + kTimeBetweenInputPackets )
	{
		SendInputPacket();
		mTimeOfLastInputPacket = time;
	}
}

void NetworkManagerClient::SendInputPacket()
{
	OutgoingPacketBuffer inputPacket; 

	inputPacket.Write( kInputCC );

	mDeliveryNotificationManager.WriteState( inputPacket );

	InputManager::sInstance->GetState().Write( inputPacket );

	SendPacket( inputPacket, mServerAddress );
}