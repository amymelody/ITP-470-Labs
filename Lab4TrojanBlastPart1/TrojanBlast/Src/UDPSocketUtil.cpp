#include <TrojanBlastPCH.h>



bool UDPSocketUtil::StaticInit()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if ( iResult != NO_ERROR ) 
	{
        ReportError (L"Starting Up");
		return false;
    }

	return true;
}

void UDPSocketUtil::CleanUp()
{
	WSACleanup();
}


void UDPSocketUtil::ReportError( const wchar_t* inOperationDesc )
{
	LPVOID lpMsgBuf;
    DWORD errorNum = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );


	LOG( L"Error %ls: %d- %ls", inOperationDesc, errorNum, lpMsgBuf );
}

int UDPSocketUtil::GetLastError() 
{ 
	return WSAGetLastError(); 
}

UDPSocketPtr UDPSocketUtil::CreateSocket( uint16_t inPort )
{
	SOCKET s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if( s == INVALID_SOCKET )
	{
		ReportError( L"Creating Socket" );
		return UDPSocketPtr();
	}

	
	SocketAddress ownAddress( INADDR_ANY, inPort );
	int bindResult = bind( s, ownAddress.GetAsSockAddr(), ownAddress.GetSize() );
	if( bindResult == SOCKET_ERROR )
	{
		ReportError( L"Binding" );
		return UDPSocketPtr();
	}
	

	//we got this far? we did it!
	return UDPSocketPtr( new UDPSocket( s ) );
}
