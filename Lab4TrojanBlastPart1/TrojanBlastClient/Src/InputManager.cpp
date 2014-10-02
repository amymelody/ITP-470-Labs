#include <TrojanBlastClientPCH.h>

unique_ptr< InputManager >	InputManager::sInstance;

void InputManager::StaticInit()
{
	sInstance.reset( new InputManager() );
}


namespace
{
	inline void UpdateDesireVariableFromKey( EInputAction inInputAction, bool& ioVariable )
	{
		if( inInputAction == EIA_Pressed )
		{
			ioVariable = true;
		}
		else if( inInputAction == EIA_Released )
		{
			ioVariable = false;
		}
	}

	inline void UpdateDesireFloatFromKey( EInputAction inInputAction, float& ioVariable )
	{
		if( inInputAction == EIA_Pressed )
		{
			ioVariable = 1.f;
		}
		else if( inInputAction == EIA_Released )
		{
			ioVariable = 0.f;
		}
	}
}

void InputManager::HandleInput( EInputAction inInputAction, int inKeyCode )
{
	switch( inKeyCode )
	{
	case 'A':
		UpdateDesireFloatFromKey( inInputAction, mCurrentState.mDesiredLeftAmount );
		break;
	case 'D':
		UpdateDesireFloatFromKey( inInputAction, mCurrentState.mDesiredRightAmount );
		break;
	case 'W':
		UpdateDesireFloatFromKey( inInputAction, mCurrentState.mDesiredForwardAmount );
		break;
	case 'S':
		UpdateDesireFloatFromKey( inInputAction, mCurrentState.mDesiredBackAmount );
		break;
	}

}


InputManager::InputManager()
{

}
