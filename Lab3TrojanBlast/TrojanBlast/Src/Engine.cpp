#include <TrojanBlastPCH.h>
#include "Engine.h"
#include <time.h>
#include "GameObjectRegistry.h"
#include "World.h"
#include "ScoreBoardManager.h"
#include "Ship.h"
#include "StringUtils.h"
#include "MathUtils.h"
#include "Timing.h"

Engine::Engine()
{
	srand( static_cast< uint32_t >( time( nullptr ) ) );
	timeElapsed = 0;

	GameObjectRegistry::StaticInit();
	World::StaticInit();
	ScoreBoardManager::StaticInit();
}

int Engine::Run()
{
	return DoRunLoop();
}

int Engine::DoRunLoop()
{
	// Main message loop
	MSG msg = {0};
	while( WM_QUIT != msg.message )
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

	timeElapsed += Timing::sInstance.GetDeltaTime();

	World::sInstance->Update();

}