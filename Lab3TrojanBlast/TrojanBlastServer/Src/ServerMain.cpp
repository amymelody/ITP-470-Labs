#include <TrojanBlastServerPCH.h>
#include "Server.h"
#include "UDPSocketUtil.h"
#include <Ws2tcpip.h>

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );

	wstring wstr(lpCmdLine);
	string portStr(wstr.begin(), wstr.end());
	USHORT inPort = atoi(portStr.c_str());

	if( Server::StaticInit(htons(inPort)) )
	{
		UDPSocketUtil::StartUp();
		return Server::sInstance->Run();
		UDPSocketUtil::CleanUp();
	}
	else
	{
		//error
		return 1;
	}
}

