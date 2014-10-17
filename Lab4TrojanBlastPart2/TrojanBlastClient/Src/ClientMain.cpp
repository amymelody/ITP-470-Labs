
#include <TrojanBlastClientPCH.h>

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

	if( Client::StaticInit( hInstance, nCmdShow ) )
	{
		return Client::sInstance->Run();
	}
	else
	{
		//error
		return 1;
	}

}

