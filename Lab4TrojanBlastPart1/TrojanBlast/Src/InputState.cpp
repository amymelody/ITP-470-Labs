#include <TrojanBlastPCH.h>

bool InputState::Write( MemoryOutputStream& inStream ) const
{
	if( inStream.GetRemainingBytes() >= 2 * sizeof( float ) )
	{

		inStream.Write( GetDesiredHorizontalDelta() );
		inStream.Write( GetDesiredVerticalDelta() );

		return true;
	}
	return false;
}

bool InputState::Read( MemoryInputStream& inStream )
{
	
	inStream.Read( mDesiredRightAmount );
	inStream.Read( mDesiredForwardAmount );


	return true;
}