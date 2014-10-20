#include <TrojanBlastClientPCH.h>

void ReplicationManagerClient::Read( IncomingPacketBuffer& inPacket )
{
	while( inPacket.GetRemainingBytes() >= 4 )
	{
		//read the network id...
		int networkId; inPacket.Read( networkId );
		
		//only need 2 bits for action...
		uint8_t action; inPacket.Read( action, 2 );

		switch( action )
		{
		case EROA_Create:
			ReadAndDoCreateAction( inPacket, networkId );
			break;
		case EROA_UpdateState:
			ReadAndDoUpdateAction( inPacket, networkId );
			break;
		case EROA_Destroy:
			ReadAndDoDestroyAction( inPacket, networkId );
			break;
		}

	}

}

void ReplicationManagerClient::ReadAndDoCreateAction( IncomingPacketBuffer& inPacket, int inNetworkId )
{
	uint32_t fourCCName;
	inPacket.Read(fourCCName);

	GameObjectPtr obj = NetworkManagerClient::sInstance->GetGameObject(inNetworkId);
	if (!obj.get()) {
		obj = GameObjectRegistry::sInstance->CreateGameObject(fourCCName);
		obj->SetNetworkId(inNetworkId);
		NetworkManagerClient::sInstance->AddToNetworkIdToGameObjectMap(obj);
	}

	obj->Read(inPacket);
}

void ReplicationManagerClient::ReadAndDoUpdateAction( IncomingPacketBuffer& inPacket, int inNetworkId )
{
	GameObjectPtr obj = NetworkManagerClient::sInstance->GetGameObject(inNetworkId);
	if (obj.get()) {
		obj->Read(inPacket);
	}
}

void ReplicationManagerClient::ReadAndDoDestroyAction( IncomingPacketBuffer& inPacket, int inNetworkId )
{
	GameObjectPtr obj = NetworkManagerClient::sInstance->GetGameObject(inNetworkId);
	if (obj.get()) {
		obj->SetDoesWantToDie(true);
		NetworkManagerClient::sInstance->RemoveFromNetworkIdToGameObjectMap(obj);
	}
}