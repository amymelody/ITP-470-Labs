#include <TrojanBlastPCH.h>

PacketBuffer::PacketBuffer()
{
	mData = new uint8_t[ kDefaultMaxPacketSize ];
	ZeroMemory( mData, kDefaultMaxPacketSize );
	mBitHead = 0;
	mBitCapacity = kDefaultMaxPacketSize << 3;
}

PacketBuffer::~PacketBuffer()
{
	delete[]( mData );
}

bool OutgoingPacketBuffer::WriteBits( uint8_t inData, uint32_t inBitCount )
{
	if( mBitHead + inBitCount <= mBitCapacity )
	{
		uint32_t byteOffset = mBitHead >> 3;
		uint32_t bitOffset = mBitHead & 0x7;

		mData[ byteOffset ] |= inData << bitOffset;

		uint32_t bitsFreeThisByte = 8 - bitOffset;
		if( bitsFreeThisByte < inBitCount )
		{
			//we need another byte…
			mData[ byteOffset + 1 ] = inData >> bitsFreeThisByte;
		}

		mBitHead += inBitCount;
		return true;
	}

	return false;
}

bool OutgoingPacketBuffer::WriteBits( const void* inData, uint32_t inBitCount )
{
	if( mBitHead + inBitCount <= mBitCapacity )
	{
		const uint8_t* srcByte = reinterpret_cast< const uint8_t* >( inData );
		//write all the bytes
		while( inBitCount > 8 )
		{
			WriteBits( *srcByte, 8 );
			++srcByte;
			inBitCount -= 8;
		}
		//write anything left
		if( inBitCount > 0 )
		{
			WriteBits( *srcByte, inBitCount );
		}
		return true;
	}

	return false;
}


bool OutgoingPacketBuffer::Write( const wstring& inData )
{
	//on windows, size of string includes null terminator if there's on passed in ( weird, eh? )
	uint32_t length = inData.size();
	uint32_t byteLength = length * 2;
	if( length < 0xffff && byteLength <= GetRemainingBytes() )
	{
		Write( static_cast< uint16_t >( length ) );
		WriteData( inData.data(), byteLength );
		return true;
	}
	return false;
}

bool IncomingPacketBuffer::ReadBits( uint8_t& outData, uint32_t inBitCount )
{
	if( mBitHead + inBitCount <= mBitCapacity )
	{
		uint32_t byteOffset = mBitHead >> 3;
		uint32_t bitOffset = mBitHead & 0x7;

		outData = mData[ byteOffset ] >> bitOffset;

		uint32_t bitsFreeThisByte = 8 - bitOffset;
		if( bitsFreeThisByte < inBitCount )
		{
			//we need another byte…
			outData |= mData[ byteOffset + 1 ] << bitsFreeThisByte;
		}

		//don't forget a mask so that we only read the bit we wanted...
		outData &= ( ~( 0x00ff << inBitCount ) );

		mBitHead += inBitCount;
		return true;
	}

	return false;
}

bool IncomingPacketBuffer::ReadBits( void* outData, uint32_t inBitCount )
{
	if( mBitHead + inBitCount <= mBitCapacity )
	{
		uint8_t* destByte = reinterpret_cast< uint8_t* >( outData );
		//write all the bytes
		while( inBitCount > 8 )
		{
			ReadBits( *destByte, 8 );
			++destByte;
			inBitCount -= 8;
		}
		//write anything left
		if( inBitCount > 0 )
		{
			ReadBits( *destByte, inBitCount );
		}
		return true;
	}

	return false;
}


bool IncomingPacketBuffer::Read( wstring& outData )
{
	uint16_t length;
	uint32_t byteLength;
	if( Read( length ) )
	{
		byteLength = length * 2;
		outData.resize( length );
		if( ReadData( &outData[ 0 ], byteLength ) )
		{
			return true;
		}
	}

	return false;
}

