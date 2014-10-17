#include <TrojanBlastPCH.h>

wstring	SocketAddress::ToString() const
{
	const sockaddr_in* s = GetAsSockAddrIn();
	return StringUtils::Sprintf( L"%d.%d.%d.%d:%d", 
								s->sin_addr.S_un.S_un_b.s_b1,
								s->sin_addr.S_un.S_un_b.s_b2, 
								s->sin_addr.S_un.S_un_b.s_b3, 
								s->sin_addr.S_un.S_un_b.s_b4,
								ntohs( s->sin_port ) );
}


SocketAddress::SocketAddress( const wstring& inString )
{

	sockaddr_in* s = GetAsSockAddrIn();
	s->sin_family = AF_INET;

	uint32_t a = 0, b = 0, c = 0, d = 0, port = 0;
	swscanf_s( inString.c_str(), L"%d.%d.%d.%d:%d", &a, &b, &c, &d, &port );

	s->sin_addr.S_un.S_un_b.s_b1 = static_cast< UCHAR >( a );
	s->sin_addr.S_un.S_un_b.s_b2 = static_cast< UCHAR >( b );
	s->sin_addr.S_un.S_un_b.s_b3 = static_cast< UCHAR >( c );
	s->sin_addr.S_un.S_un_b.s_b4 = static_cast< UCHAR >( d );
	s->sin_port = htons( static_cast< uint16_t >( port ) );

	
}