#include <TrojanBlastPCH.h>
#include <time.h>

std::unique_ptr< Engine >	Engine::sInstance;


Engine::Engine() :
mShouldKeepRunning( true )
{
	UDPSocketUtil::StaticInit();

	srand( static_cast< uint32_t >( time( nullptr ) ) );
	
	GameObjectRegistry::StaticInit();


	World::StaticInit();

	ScoreBoardManager::StaticInit();
}

Engine::~Engine()
{
	UDPSocketUtil::CleanUp();
}




int Engine::Run()
{
	return DoRunLoop();
}

int Engine::DoRunLoop()
{
	// Main message loop
	MSG msg = {0};
	while( WM_QUIT != msg.message && mShouldKeepRunning )
	{
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			DoFrame();
		}
	}

	return ( int )msg.wParam;
}

void Engine::DoFrame()
{
  
	Timing::sInstance.Update();

	World::sInstance->Update();
}

	
