#include <TrojanBlastPCH.h>

namespace
{
	void WriteSignedBinaryValue( OutgoingPacketBuffer& inPacket, float inValue )
	{
		bool isNonZero = ( inValue != 0.f );
		inPacket.Write( isNonZero );
		if( isNonZero )
		{
			inPacket.Write( inValue > 0.f );
		}
	}

	void ReadSignedBinaryValue( IncomingPacketBuffer& inPacket, float& outValue )
	{
		bool isNonZero;
		inPacket.Read( isNonZero );
		if( isNonZero )
		{
			bool isPositive;
			inPacket.Read( isPositive );
			outValue = isPositive ? 1.f : -1.f;
		}
		else
		{
			outValue = 0.f;
		}
	}
}

bool InputState::Write( OutgoingPacketBuffer& inPacket ) const
{
	if( inPacket.GetRemainingBytes() >= 2 * sizeof( float ) )
	{
		WriteSignedBinaryValue( inPacket, GetDesiredHorizontalDelta() );
		WriteSignedBinaryValue( inPacket, GetDesiredVerticalDelta() );
		return true;
	}
	return false;
}

bool InputState::Read( IncomingPacketBuffer& inPacket )
{
	
	ReadSignedBinaryValue( inPacket, mDesiredRightAmount );
	ReadSignedBinaryValue( inPacket, mDesiredForwardAmount );

	return true;
}