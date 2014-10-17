#include <TrojanBlastServerPCH.h>

void ReplicationManagerTransmissionData::AddTransmission( int inNetworkId, EReplicatedObjectAction inAction, uint32_t inState )
{
	/*
	//it would be silly if we already had a transmission for this network id in here...
	for( const auto& transmission: mTransmissions )
	{
		assert( inNetworkId != transmission.GetNetworkId() );
	}
	*/
	mTransmissions.emplace_back( inNetworkId, inAction, inState );
}

void ReplicationManagerTransmissionData::HandleDeliveryFailure( DeliveryNotificationManager* inDeliveryNotificationManager ) const
{
	//run through the transmissions
	for( const ReplicationTransmission& rt: mTransmissions )
	{
		//is it a create? then we have to redo the create.
		int networkId = rt.GetNetworkId();

		switch( rt.GetAction() )
		{
		case EROA_Create:
			HandleCreateDeliveryFailure( networkId );
			break;
		case EROA_UpdateState:
			HandleUpdateStateDeliveryFailure( networkId, rt.GetState(), inDeliveryNotificationManager );
			break;
		case EROA_Destroy:
			HandleDestroyDeliveryFailure( networkId );
			break;
		}
		
	}
}

void ReplicationManagerTransmissionData::HandleDeliverySuccess( DeliveryNotificationManager* inDeliveryNotificationManager ) const
{
	//run through the transmissions, if any are Destroyed then we can remove this network id from the map
	for( const ReplicationTransmission& rt: mTransmissions )
	{
		switch( rt.GetAction() )
		{
		case EROA_Create:
			HandleCreateDeliverySuccess( rt.GetNetworkId() );
			break;
		case EROA_Destroy:
			HandleDestroyDeliverySuccess( rt.GetNetworkId() );
			break;
		}
	}
}




void ReplicationManagerTransmissionData::HandleCreateDeliveryFailure( int inNetworkId ) const
{
	//lab4 part 2

	//does the object still exist? it might be dead, in which case we don't resend a create
	//otherwise, replicate the create again!
}

void ReplicationManagerTransmissionData::HandleDestroyDeliveryFailure( int inNetworkId ) const
{
	//lab 4 part 2
	
	//replicate the destroy again!
}

void ReplicationManagerTransmissionData::HandleUpdateStateDeliveryFailure( int inNetworkId, uint32_t inState, DeliveryNotificationManager* inDeliveryNotificationManager ) const
{
	//lab 4 part 2
	//does the object still exist? it might be dead, in which case we don't resend an update
	//if it's still alive,
	//get the inflight packet list from the DeliveryNotificaitonManager, 
	//run through the inflight packets  to see if any contain updates for this network object, 
	//and if so, remove the state bits that are updated in them from the state bits that you need to resend. 
	//When you’re all done, if there any state left to resend, 
	//call SetStateDirty on the mReplicationManagerServer with the remaining state bits.

}

void ReplicationManagerTransmissionData::HandleCreateDeliverySuccess( int inNetworkId ) const
{
	//we've received an ack for the create, so we can start sending as only an update
	mReplicationManagerServer->HandleCreateAckd( inNetworkId );
}

void ReplicationManagerTransmissionData::HandleDestroyDeliverySuccess( int inNetworkId ) const
{
	mReplicationManagerServer->RemoveFromReplication( inNetworkId );
}