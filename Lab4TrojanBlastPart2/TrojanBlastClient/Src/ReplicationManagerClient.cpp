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
	//lab4 part2
}

void ReplicationManagerClient::ReadAndDoUpdateAction( IncomingPacketBuffer& inPacket, int inNetworkId )
{
	//lab4 part2
	
}

void ReplicationManagerClient::ReadAndDoDestroyAction( IncomingPacketBuffer& inPacket, int inNetworkId )
{
	//lab4 part2

	//if something was destroyed before the create went through, we'll never get it
	//but we might get the destroy request, so be tolerant of being asked to destroy something that wasn't created
	
}