#include <TrojanBlastPCH.h>

bool InputState::Write( MemoryOutputStream& inStream ) const
{
	if( inStream.GetRemainingBytes() >= 2 * sizeof( float ) )
	{
		uint8_t hSign = GetDesiredHorizontalDelta() < 0;
		uint8_t hVal = abs(GetDesiredHorizontalDelta());
		uint8_t vSign = GetDesiredVerticalDelta() < 0;
		uint8_t vVal = abs(GetDesiredVerticalDelta());

		inStream.WriteBits(hSign, 1);
		inStream.WriteBits(hVal, 1);
		inStream.WriteBits(vSign, 1);
		inStream.WriteBits(vVal, 1);

		return true;
	}
	return false;
}

bool InputState::Read( MemoryInputStream& inStream )
{
	uint8_t hSign;
	uint8_t hVal;
	uint8_t vSign;
	uint8_t vVal;

	inStream.ReadBits(hSign, 1);
	inStream.ReadBits(hVal, 1);
	inStream.ReadBits(vSign, 1);
	inStream.ReadBits(vVal, 1);

	mDesiredRightAmount = hVal * (1 - 2 * hSign);
	mDesiredForwardAmount = vVal * (1 - 2 * vSign);

	return true;
}