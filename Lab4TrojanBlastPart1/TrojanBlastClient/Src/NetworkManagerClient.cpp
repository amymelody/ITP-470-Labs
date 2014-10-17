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

void NetworkManagerClient::ProcessPacket( MemoryInputStream& inMemoryStream, const SocketAddress& inFromAddress )
{
	uint32_t	packetType;
	if( inMemoryStream.Read( packetType ) )
	{
		switch( packetType )
		{
		case kWelcomeCC:
			HandleWelcomePacket( inMemoryStream );
			break;
		case kStateCC:
			if( mDeliveryNotificationManager.ReadAndProcessState( inMemoryStream ) )
			{
				HandleStatePacket( inMemoryStream );
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
	MemoryOutputStream helloPacket; 

	helloPacket.Write( kHelloCC );
	helloPacket.Write( mName );

	SendPacket( helloPacket, mServerAddress );
}

void NetworkManagerClient::HandleWelcomePacket( MemoryInputStream& inMemoryStream )
{
	//if we got a player id, we've been welcomed!
	int playerId;
	if( inMemoryStream.Read( playerId ) )
	{
		mPlayerId = playerId;
		mState = NCS_Welcomed;
		LOG( L"'%ls' was welcomed on client as player %d", mName.c_str(), mPlayerId );
	}
}



void NetworkManagerClient::HandleStatePacket( MemoryInputStream& inMemoryStream )
{
	HandleGameObjectState( inMemoryStream );
	HandleScoreBoardState( inMemoryStream );
}



void NetworkManagerClient::HandleGameObjectState( MemoryInputStream& inMemoryStream )
{
	//copy the mNetworkIdToGameObjectMap so that anything that doesn't get an updated can be destroyed...
	IntToGameObjectMap	objectsToDestroy = mNetworkIdToGameObjectMap;

	int stateCount;
	if( inMemoryStream.Read( stateCount ) && stateCount > 0 )
	{
		for( int stateIndex = 0; stateIndex < stateCount; ++stateIndex )
		{
			int networkId;
			uint32_t fourCC;

			if( inMemoryStream.Read( networkId ) )
			{
				if( inMemoryStream.Read( fourCC ) )
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
					go->Read( inMemoryStream );
					objectsToDestroy.erase( networkId );
				}
			}
		}
	}

	//anything left gets the axe
	DestroyGameObjectsInMap( objectsToDestroy );
}

void NetworkManagerClient::HandleScoreBoardState( MemoryInputStream& inMemoryStream )
{
	ScoreBoardManager::sInstance->Read( inMemoryStream );
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

void NetworkManagerClient::AddToNetworkIdToGameObjectMap( GameObjectPtr inGameObject )
{
	mNetworkIdToGameObjectMap[ inGameObject->GetNetworkId() ] = inGameObject;
}

void NetworkManagerClient::RemoveFromNetworkIdToGameObjectMap( GameObjectPtr inGameObject )
{
	mNetworkIdToGameObjectMap.erase( inGameObject->GetNetworkId() );
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
	MemoryOutputStream inputPacket;

	inputPacket.Write( kInputCC );

	mDeliveryNotificationManager.WriteState( inputPacket );

	const InputState& currentState = InputManager::sInstance->GetState();
	if (currentState.GetDesiredHorizontalDelta() != mPreviousState.GetDesiredHorizontalDelta() ||
		currentState.GetDesiredVerticalDelta() != mPreviousState.GetDesiredVerticalDelta()) {
		bool isInput = true;
		inputPacket.WriteBits(isInput, 1);
		InputManager::sInstance->GetState().Write(inputPacket);
		mPreviousState = currentState;
	}
	else {
		bool isInput = false;
		inputPacket.WriteBits(isInput, 1);
	}

	SendPacket( inputPacket, mServerAddress );
}