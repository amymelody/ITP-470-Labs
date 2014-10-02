class UDPSocket
{
public:

	friend class UDPSocketUtil;

	~UDPSocket();

	int SendTo( const MemoryOutputStream& inMemoryStream, const SocketAddress& inToAddress );
	int ReceiveFrom( MemoryInputStream& outMemoryStream, SocketAddress& outFromAddress );

	int SetNonBlockingMode( bool inShouldBeNonBlocking );

private:
	UDPSocket( SOCKET inSocket ) : mSocket( inSocket ) {}

	SOCKET mSocket;

};

typedef shared_ptr< UDPSocket >	UDPSocketPtr;