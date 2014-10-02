#include <TrojanBlastPCH.h>

int UDPSocket::SendTo( const MemoryOutputStream& inMemoryStream, const SocketAddress& inToAddress )
{
	int byteSentCount = sendto( mSocket, 
								reinterpret_cast< const char* >( inMemoryStream.GetData() ), 
								inMemoryStream.GetByteLength(), 
								0, inToAddress.GetAsSockAddr(), inToAddress.GetSize() );
	if( byteSentCount <= 0 )
	{
		//we'll return error as negative number to indicate less than requested amount of bytes sent...
		UDPSocketUtil::ReportError( L"UDPSocket::SendTo" );
		return -UDPSocketUtil::GetLastError();
	}
	else
	{
		//great, it all sent find...
		return byteSentCount;
	}
}

int UDPSocket::ReceiveFrom( MemoryInputStream& outMemoryStream, SocketAddress& outFromAddress )
{
	int fromLength = outFromAddress.GetSize();
	int readByteCount = recvfrom( mSocket, 
									reinterpret_cast< char* >( outMemoryStream.GetData() ), 
									MemoryInputStream::kDefaultMaxPacketSize, 
									0, outFromAddress.GetAsSockAddr(), &fromLength );
	if( readByteCount >= 0 )
	{
		outMemoryStream.ResetToCapacity( readByteCount );
		return readByteCount;
	}
	else
	{
		int error = UDPSocketUtil::GetLastError();

		if( error == WSAEWOULDBLOCK )
		{
			//that's okay, just nothing received...
			outMemoryStream.ResetToCapacity( 0 );
			return 0;
		}
		else if( error == WSAECONNRESET )
		{
			//this can happen if a client closed and we haven't DC'd yet.
			//this is the ICMP message being sent back saying the port on that computer is closed
			LOG( L"Connection reset from %ls", outFromAddress.ToString().c_str() );
			return -WSAECONNRESET;
		}
		else
		{
			//uhoh, error...
			outMemoryStream.ResetToCapacity( 0 );
			UDPSocketUtil::ReportError( L"UDPSocket::ReceiveFrom" );
			return -error;
		}
	}

}

int UDPSocket::SetNonBlockingMode( bool inShouldBeNonBlocking )
{
	u_long arg = inShouldBeNonBlocking ? 1 : 0;
	int result = ioctlsocket( mSocket, FIONBIO, &arg );
	if( result == SOCKET_ERROR )
	{
		UDPSocketUtil::ReportError( L"UDPSocket::SetNonBlockingMode" );
		return UDPSocketUtil::GetLastError(); 
	}
	else
	{
		return NO_ERROR;
	}


}


UDPSocket::~UDPSocket()
{
	closesocket( mSocket );
}