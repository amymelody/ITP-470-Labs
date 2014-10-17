#include <TrojanBlastServerPCH.h>

void ReplicationManagerServer::ReplicateCreate( int inNetworkId, uint32_t inInitialDirtyState )
{
	mNetworkIdToReplicationCommand[ inNetworkId ] = ReplicationCommand( inInitialDirtyState );
}

void ReplicationManagerServer::ReplicateDestroy( int inNetworkId )
{
	//it's broken if we don't find it...
	mNetworkIdToReplicationCommand[ inNetworkId ].SetDestroy();
}

void ReplicationManagerServer::RemoveFromReplication( int inNetworkId )
{
	mNetworkIdToReplicationCommand.erase( inNetworkId );
}

void ReplicationManagerServer::SetStateDirty( int inNetworkId, uint32_t inDirtyState )
{
	mNetworkIdToReplicationCommand[ inNetworkId ].AddDirtyState( inDirtyState );
}

void ReplicationManagerServer::HandleCreateAckd( int inNetworkId )
{
	mNetworkIdToReplicationCommand[ inNetworkId ].HandleCreateAckd();
}

void ReplicationManagerServer::Write( OutgoingPacketBuffer& inPacket, ReplicationManagerTransmissionData* ioTransmissinData )
{
	//run through each replication command and do something...
	for( auto& pair: mNetworkIdToReplicationCommand )
	{
		if( inPacket.GetRemainingBytes() > 4 )
		{
			ReplicationCommand& replicationCommand = pair.second;
			if( replicationCommand.HasDirtyState() )
			{
				int networkId = pair.first;

				//well, first write the network id...
				inPacket.Write( networkId );

				//only need 2 bits for action...
				EReplicatedObjectAction action = replicationCommand.GetAction();
				inPacket.Write( action, 2 );

				uint32_t writtenState = 0;
				uint32_t dirtyState = replicationCommand.GetDirtyState();

				//now do what?
				switch( action )
				{
				case EROA_Create:
					writtenState = WriteCreateAction( inPacket, networkId, dirtyState );
					break;
				case EROA_UpdateState:
					writtenState = WriteUpdateAction( inPacket, networkId, dirtyState );
					break;
				case EROA_Destroy:
					//don't need anything other than state!
					writtenState = WriteDestroyAction( inPacket, networkId, dirtyState );
					break;
				}
		
				ioTransmissinData->AddTransmission( networkId, action, writtenState );

				//let's pretend everything was written- don't make this too hard
				replicationCommand.ClearDirtyState( writtenState );

			}
		}
		else
		{
			//uhoh, no more room!
			return;
		}
	}
}


uint32_t ReplicationManagerServer::WriteCreateAction( OutgoingPacketBuffer& inPacket, int inNetworkId, uint32_t inDirtyState )
{
	//need object
	GameObjectPtr gameObject = NetworkManagerServer::sInstance->GetGameObject( inNetworkId );
	//need 4 cc
	inPacket.Write( gameObject->GetFourCCName() );
	return gameObject->Write( inPacket, inDirtyState );
}

uint32_t ReplicationManagerServer::WriteUpdateAction( OutgoingPacketBuffer& inPacket, int inNetworkId, uint32_t inDirtyState )
{
	//need object
	GameObjectPtr gameObject = NetworkManagerServer::sInstance->GetGameObject( inNetworkId );

	uint32_t writtenState = gameObject->Write( inPacket, inDirtyState );

	return writtenState;
}

uint32_t ReplicationManagerServer::WriteDestroyAction( OutgoingPacketBuffer& inPacket, int inNetworkId, uint32_t inDirtyState )
{
	( void ) inPacket;
	( void ) inNetworkId;
	( void ) inDirtyState;
	//don't have to do anything- action already written

	return inDirtyState;
}