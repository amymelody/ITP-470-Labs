class SocketAddress
{


public:
	
	SocketAddress()
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = 0;
		GetAsSockAddrIn()->sin_port = 0;
	}

	SocketAddress( const wstring& inString );

	SocketAddress( uint32_t inAddress, uint16_t inPort )
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = inAddress;
		GetAsSockAddrIn()->sin_port = htons( inPort );
	}

	bool operator==( const SocketAddress& inOther ) const
	{
		return ( GetAsSockAddrIn()->sin_port == inOther.GetAsSockAddrIn()->sin_port ) && 
				( GetAsSockAddrIn()->sin_addr.S_un.S_addr == inOther.GetAsSockAddrIn()->sin_addr.S_un.S_addr );
	}

			sockaddr*		GetAsSockAddr()				{ return &mSockAddr; }
	const	sockaddr*		GetAsSockAddr()		const	{ return &mSockAddr; }

			sockaddr_in*	GetAsSockAddrIn()			{ return reinterpret_cast< sockaddr_in* >( &mSockAddr ); }
	const	sockaddr_in*	GetAsSockAddrIn()	const	{ return reinterpret_cast< const sockaddr_in* >( &mSockAddr ); }

	uint32_t				GetSize()			const	{ return sizeof( sockaddr ); }

	wstring					ToString()			const;

private:
	sockaddr mSockAddr;
};

namespace std
{
	template<> struct hash< SocketAddress >
	{
		size_t operator()( const SocketAddress& inAddress )
		{
			return ( inAddress.GetAsSockAddrIn()->sin_addr.S_un.S_addr ) | ( ( static_cast< uint32_t >( inAddress.GetAsSockAddrIn()->sin_port ) ) << 13 );
		}
	};
}