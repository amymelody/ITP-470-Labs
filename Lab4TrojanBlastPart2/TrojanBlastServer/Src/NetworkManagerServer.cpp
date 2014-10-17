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

void NetworkManagerServer::ProcessPacket( IncomingPacketBuffer& inPacketBuffer, const SocketAddress& inFromAddress )
{
	//try to get the client proxy for this address
	//pass this to the client proxy to process
	auto it = mAddressToClientMap.find( inFromAddress );
	if( it == mAddressToClientMap.end() )
	{
		//didn't find one? it's a new cilent..is the a HELO? if so, create a client proxy...
		HandlePacketFromNewClient( inPacketBuffer, inFromAddress );
	}
	else
	{
		ProcessPacket( ( *it ).second, inPacketBuffer );
	}
}


void NetworkManagerServer::ProcessPacket( ClientProxyPtr inClientProxy, IncomingPacketBuffer& inPacketBuffer )
{
	//remember we got a packet so we know not to disconnect for a bit
	inClientProxy->UpdateLastPacketTime();

	uint32_t	packetType;
	if( inPacketBuffer.Read( packetType ) )
	{
		switch( packetType )
		{
		case kHelloCC:
			//need to resend welcome. to be extra safe we should check the name is the one we expect from this address,
			//otherwise something weird is going on...
			SendWelcomePacket( inClientProxy );
			break;
		case kInputCC:
			if( inClientProxy->GetDeliveryNotificationManager().ReadAndProcessState( inPacketBuffer ) )
			{
				HandleInputPacket( inClientProxy, inPacketBuffer );
			}
			break;
		default:
			LOG( L"Unknown packet type received from %s", inClientProxy->GetSocketAddress().ToString().c_str() );
			break;
		}
	}
}


void NetworkManagerServer::HandlePacketFromNewClient( IncomingPacketBuffer& inPacketBuffer, const SocketAddress& inFromAddress )
{
	//read the beginning- is it a hello?
	uint32_t	packetType;
	if( inPacketBuffer.Read( packetType ) && packetType == kHelloCC )
	{
		//read the name
		wstring name;
		if( inPacketBuffer.Read( name ) )
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

			//and now init the replication manager with everything we know about!
			for( const auto& pair: mNetworkIdToGameObjectMap )
			{
				newClientProxy->GetReplicationManagerServer().ReplicateCreate( pair.first, pair.second->GetAllStateMask() );
			}
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
	OutgoingPacketBuffer welcomePacket; 

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
	OutgoingPacketBuffer	statePacket;

	//it's state!
	statePacket.Write( kStateCC );

	DeliveryNotificationManager::InFlightPacket* ifp = inClientProxy->GetDeliveryNotificationManager().WriteState( statePacket );

	//old...
	//AddWorldStateToPacket( statePacket );
	AddScoreBoardStateToPacket( statePacket );

	ReplicationManagerTransmissionData* rmtd = new ReplicationManagerTransmissionData( &inClientProxy->GetReplicationManagerServer() );
	inClientProxy->GetReplicationManagerServer().Write( statePacket, rmtd );
	ifp->SetReplicationTransmissionData( DeliveryNotificationManager::TransmissionDataPtr( rmtd ) );

	SendPacket( statePacket, inClientProxy->GetSocketAddress() );
	
}

//should we ask the server for this? or run through the world ourselves?
void NetworkManagerServer::AddWorldStateToPacket( OutgoingPacketBuffer& inPacket )
{
	const auto& gameObjects = World::sInstance->GetGameObjects();

	//now start writing objects- do we need to remember how many there are? we can check first...
	inPacket.Write( gameObjects.size() );

	for( GameObjectPtr gameObject : gameObjects )
	{
		inPacket.Write( gameObject->GetNetworkId() );
		inPacket.Write( gameObject->GetFourCCName() );
		gameObject->Write( inPacket, 0xffffffff );
	}
}

void NetworkManagerServer::AddScoreBoardStateToPacket( OutgoingPacketBuffer& inPacket )
{
	ScoreBoardManager::sInstance->Write( inPacket );
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

void NetworkManagerServer::HandleInputPacket( ClientProxyPtr inClientProxy, IncomingPacketBuffer& inPacketBuffer )
{
	InputState	inputState;
	if( inputState.Read( inPacketBuffer ) )
	{
		inClientProxy->SetInputState( inputState );
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

void NetworkManagerServer::RegisterGameObject( GameObjectPtr inGameObject )
{
	//assign network id
	int newNetworkId = GetNewNetworkId();
	inGameObject->SetNetworkId( newNetworkId );

	//add mapping from network id to game object
	mNetworkIdToGameObjectMap[ newNetworkId ] = inGameObject;

	//tell all client proxies this is new...
	for( const auto& pair: mAddressToClientMap )
	{
		pair.second->GetReplicationManagerServer().ReplicateCreate( newNetworkId, inGameObject->GetAllStateMask() );
	}
}


void NetworkManagerServer::UnregisterGameObject( GameObject* inGameObject )
{
	int networkId = inGameObject->GetNetworkId();
	mNetworkIdToGameObjectMap.erase( networkId );

	//tell all client proxies to STOP replicating!
	//tell all client proxies this is new...
	for( const auto& pair: mAddressToClientMap )
	{
		pair.second->GetReplicationManagerServer().ReplicateDestroy( networkId );
	}
}

void NetworkManagerServer::SetStateDirty( int inNetworkId, uint32_t inDirtyState )
{
	//tell everybody this is dirty
	for( const auto& pair: mAddressToClientMap )
	{
		pair.second->GetReplicationManagerServer().SetStateDirty( inNetworkId, inDirtyState );
	}
}

