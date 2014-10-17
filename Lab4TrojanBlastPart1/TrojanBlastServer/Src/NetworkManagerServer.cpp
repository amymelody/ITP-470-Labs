#include <TrojanBlastServerPCH.h>

NetworkManagerServer*	NetworkManagerServer::sInstance;


NetworkManagerServer::NetworkManagerServer() :
	mNewPlayerId( 1 ),
	mNewNetworkId( 1 ),
	mTimeBetweenStatePackets( 0.033f ),
	mClientDisconnectTimeout( 100.f )
{
}

bool NetworkManagerServer::StaticInit( uint16_t inPort )
{
	sInstance = new NetworkManagerServer();
	return sInstance->Init( inPort );
}

void NetworkManagerServer::HandleConnectionReset( const SocketAddress& inFromAddress )
{
	//just dc the client right away...
	auto it = mAddressToClientMap.find( inFromAddress );
	if( it != mAddressToClientMap.end() )
	{
		HandleClientDisconnected( it->second );
	}
}

void NetworkManagerServer::ProcessPacket( MemoryInputStream& inMemoryStream, const SocketAddress& inFromAddress )
{
	//try to get the client proxy for this address
	//pass this to the client proxy to process
	auto it = mAddressToClientMap.find( inFromAddress );
	if( it == mAddressToClientMap.end() )
	{
		//didn't find one? it's a new cilent..is the a HELO? if so, create a client proxy...
		HandlePacketFromNewClient( inMemoryStream, inFromAddress );
	}
	else
	{
		ProcessPacket( ( *it ).second, inMemoryStream );
	}
}


void NetworkManagerServer::ProcessPacket( ClientProxyPtr inClientProxy, MemoryInputStream& inMemoryStream )
{
	//remember we got a packet so we know not to disconnect for a bit
	inClientProxy->UpdateLastPacketTime();

	uint32_t	packetType;
	if( inMemoryStream.Read( packetType ) )
	{
		switch( packetType )
		{
		case kHelloCC:
			//need to resend welcome. to be extra safe we should check the name is the one we expect from this address,
			//otherwise something weird is going on...
			SendWelcomePacket( inClientProxy );
			break;
		case kInputCC:
			if( inClientProxy->GetDeliveryNotificationManager().ReadAndProcessState( inMemoryStream ) )
			{
				HandleInputPacket( inClientProxy, inMemoryStream );
			}
			break;
		default:
			LOG( L"Unknown packet type received from %s", inClientProxy->GetSocketAddress().ToString().c_str() );
			break;
		}
	}
}


void NetworkManagerServer::HandlePacketFromNewClient( MemoryInputStream& inMemoryStream, const SocketAddress& inFromAddress )
{
	//read the beginning- is it a hello?
	uint32_t	packetType;
	if( inMemoryStream.Read( packetType ) && packetType == kHelloCC )
	{
		//read the name
		wstring name;
		if( inMemoryStream.Read( name ) )
		{
			ClientProxyPtr newClientProxy = std::make_shared< ClientProxy >( inFromAddress, name, mNewPlayerId++ );
			mAddressToClientMap[ inFromAddress ] = newClientProxy;
			mPlayerIdToClientMap[ newClientProxy->GetPlayerId() ] = newClientProxy;
		
			//tell the server about this client, spawn a ship, etc...
			//if we had a generic message system, this would be a good use for it...
			//instead we'll just tell the server directly
			static_cast< Server* > ( Engine::sInstance.get() )->HandleNewClient( newClientProxy );

			//and welcome the client...
			SendWelcomePacket( newClientProxy );

		}
	}
	else
	{
		//bad incoming packet from unknown client- we're under attack!!
		LOG( L"Bad incoming packet from unkown client at socket %ls", inFromAddress.ToString().c_str() );
	}
}

void NetworkManagerServer::SendWelcomePacket( ClientProxyPtr inClientProxy )
{
	MemoryOutputStream welcomePacket; 

	welcomePacket.Write( kWelcomeCC );
	welcomePacket.Write( inClientProxy->GetPlayerId() );

	LOG( L"Server Welcoming, new client '%ls' as player %d", inClientProxy->GetName().c_str(), inClientProxy->GetPlayerId() );

	SendPacket( welcomePacket, inClientProxy->GetSocketAddress() );
}

void NetworkManagerServer::SendOutgoingPackets()
{
	float time = Timing::sInstance.GetTimef();

	//we could eventually even have a different state update rate for each client
	if( time > mTimeOfLastSatePacket + mTimeBetweenStatePackets )
	{
		UpdateAllClients();
		mTimeOfLastSatePacket = time;
	}
}

void NetworkManagerServer::UpdateAllClients()
{
	for( auto it = mAddressToClientMap.begin(), end = mAddressToClientMap.end(); it != end; ++it )
	{
		//process any timed out packets while we're going throug hthe list
		it->second->GetDeliveryNotificationManager().ProcessTimedOutPackets();

		SendStatePacketToClient( it->second );
	}
}

void NetworkManagerServer::SendStatePacketToClient( ClientProxyPtr inClientProxy )
{
	//build state packet
	MemoryOutputStream	statePacket;

	//it's state!
	statePacket.Write( kStateCC );

	inClientProxy->GetDeliveryNotificationManager().WriteState( statePacket );

	AddWorldStateToPacket( statePacket );
	AddScoreBoardStateToPacket( statePacket );

	SendPacket( statePacket, inClientProxy->GetSocketAddress() );
	
}

//should we ask the server for this? or run through the world ourselves?
void NetworkManagerServer::AddWorldStateToPacket( MemoryOutputStream& inStream )
{
	const auto& gameObjects = World::sInstance->GetGameObjects();

	//now start writing objects- do we need to remember how many there are? we can check first...
	inStream.Write( gameObjects.size() );

	for( GameObjectPtr gameObject : gameObjects )
	{
		inStream.Write( gameObject->GetNetworkId() );
		inStream.Write( gameObject->GetFourCCName() );
		gameObject->Write( inStream );
	}
}

void NetworkManagerServer::AddScoreBoardStateToPacket( MemoryOutputStream& inStream )
{
	ScoreBoardManager::sInstance->Write( inStream );
}


int NetworkManagerServer::GetNewNetworkId()
{
	int toRet = mNewNetworkId++; 
	if( mNewNetworkId < toRet )
	{
		LOG( L"Network ID Wrap Around!!! You've been playing way too long...", 0 );
	}

	return toRet;

}

void NetworkManagerServer::HandleInputPacket( ClientProxyPtr inClientProxy, MemoryInputStream& inMemoryStream )
{
	uint8_t isInput;
	inMemoryStream.ReadBits(isInput, 1);
	if (isInput) {
		InputState	inputState;
		if (inputState.Read(inMemoryStream))
		{
			inClientProxy->SetInputState(inputState);
		}
	}
}

ClientProxyPtr NetworkManagerServer::GetClientProxy( int inPlayerId ) const
{
	auto it = mPlayerIdToClientMap.find( inPlayerId );
	if( it != mPlayerIdToClientMap.end() )
	{
		return it->second;
	}

	return nullptr;
}

void NetworkManagerServer::CheckForDisconnects()
{
	vector< ClientProxyPtr > clientsToDC;

	float minAllowedLastPacketFromClientTime = Timing::sInstance.GetTimef() - mClientDisconnectTimeout;
	for( const auto& pair: mAddressToClientMap )
	{
		if( pair.second->GetLastPacketFromClientTime() < minAllowedLastPacketFromClientTime )
		{
			//can't remove from map while in iterator, so just remember for later...
			clientsToDC.push_back( pair.second );
		}
	}

	for( ClientProxyPtr client: clientsToDC )
	{
		HandleClientDisconnected( client );
	}
}

void NetworkManagerServer::HandleClientDisconnected( ClientProxyPtr inClientProxy )
{
	mPlayerIdToClientMap.erase( inClientProxy->GetPlayerId() );
	mAddressToClientMap.erase( inClientProxy->GetSocketAddress() );
	static_cast< Server* > ( Engine::sInstance.get() )->HandleLostClient( inClientProxy );

	//was that the last client? if so, bye!
	if( mAddressToClientMap.empty() )
	{
		Engine::sInstance->SetShouldKeepRunning( false );
	}
}

