class ReplicationManagerClient
{
public:
	void Read( IncomingPacketBuffer& inPacket );

private:

	void ReadAndDoCreateAction( IncomingPacketBuffer& inPacket, int inNetworkId );
	void ReadAndDoUpdateAction( IncomingPacketBuffer& inPacket, int inNetworkId );
	void ReadAndDoDestroyAction( IncomingPacketBuffer& inPacket, int inNetworkId );

};