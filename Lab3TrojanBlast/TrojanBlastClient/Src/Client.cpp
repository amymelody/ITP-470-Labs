
#include <TrojanBlastClientPCH.h>

bool Client::StaticInit( HINSTANCE hInstance, int inCmdShow )
{
	if( FAILED( WindowManager::StaticInit( hInstance, inCmdShow ) ) )
	{
        return false;
	}

    if( FAILED( GraphicsDriver::StaticInit( WindowManager::sInstance->GetMainWindow() ) ) )
	{
        return false;
    }

	sInstance.reset( new Client() );

	return true;
}