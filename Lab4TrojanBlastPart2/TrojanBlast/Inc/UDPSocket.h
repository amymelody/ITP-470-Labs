class UDPSocket
{
public:

	friend class UDPSocketUtil;

	~UDPSocket();

	int SendTo( const OutgoingPacketBuffer& inPacketBuffer, const SocketAddress& inToAddress );
	int ReceiveFrom( IncomingPacketBuffer& outPacketBuffer, SocketAddress& outFromAddress );

	int SetNonBlockingMode( bool inShouldBeNonBlocking );

private:
	UDPSocket( SOCKET inSocket ) : mSocket( inSocket ) {}

	SOCKET mSocket;

};

typedef shared_ptr< UDPSocket >	UDPSocketPtr;