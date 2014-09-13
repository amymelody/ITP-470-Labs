#include <TrojanBlastServerPCH.h>
#include "Server.h"

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

	if( Server::StaticInit() )
	{
		return Server::sInstance->Run();
	}
	else
	{
		//error
		return 1;
	}
}

