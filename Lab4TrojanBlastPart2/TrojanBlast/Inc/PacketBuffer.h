


class PacketBuffer
{
public:
	static const int kDefaultMaxPacketSize = 1400;

	PacketBuffer();
	virtual ~PacketBuffer();

	const	uint8_t*	GetData()			const	{ return mData; }
			uint8_t*	GetData()					{ return mData; }

			//add 7 because the shift truncates and if we're just one bit over we need a full extra byte!
			uint32_t	GetByteLength()		const	{ return ( mBitHead + 7 ) >> 3; }
			uint32_t	GetBitHead()		const	{ return mBitHead; }

			uint32_t	GetRemainingBytes()	const	{ return ( mBitCapacity - mBitHead ) >> 3; }

protected:

	uint8_t*	mData;
	uint32_t	mBitHead;
	uint32_t	mBitCapacity;
};

class OutgoingPacketBuffer : public PacketBuffer
{
public:

	bool		WriteBits( uint8_t inData, uint32_t inBitCount );
	bool		WriteBits( const void* inData, uint32_t inBitCount );

	bool		WriteData( const void* inData, uint32_t inByteCount )	{ return WriteBits( inData, inByteCount << 3 ); }

	bool		Write( const XMVECTOR& inData )							{ return WriteData( &inData, sizeof( XMVECTOR ) ); }
	bool		Write( const XMVECTORF32& inData )						{ return WriteData( &inData, sizeof( XMVECTORF32 ) ); }

	bool		Write( uint32_t inData, uint32_t inBitCount = 32 )	{ return WriteBits( &inData, inBitCount ); }
	bool		Write( int inData, uint32_t inBitCount = 32 )		{ return WriteBits( &inData, inBitCount ); }
	bool		Write( float inData )								{ return WriteBits( &inData, 32 ); }

	bool		Write( uint16_t inData, uint32_t inBitCount = 16 )	{ return WriteBits( &inData, inBitCount ); }
	bool		Write( int16_t inData, uint32_t inBitCount = 16 )	{ return WriteBits( &inData, inBitCount ); }

	bool		Write( uint8_t inData, uint32_t inBitCount = 8 )	{ return WriteBits( &inData, inBitCount ); }
	
	bool		Write( bool inData )								{ return WriteBits( &inData, 1 ); }

	bool		Write( const wstring& inData );
};

class IncomingPacketBuffer : public PacketBuffer
{
public:
	
	bool		ReadBits( uint8_t& outData, uint32_t inBitCount );
	bool		ReadBits( void* outData, uint32_t inBitCount );

	bool		ReadData( void* outData, uint32_t inByteCount )			{ return ReadBits( outData, inByteCount << 3 ); }

	bool		Read( XMVECTOR& outData )								{ return ReadData( &outData, sizeof( XMVECTOR ) ); }
	bool		Read( XMVECTORF32& outData )							{ return ReadData( &outData, sizeof( XMVECTORF32 ) ); }

	bool		Read( uint32_t& outData, uint32_t inBitCount = 32 )		{ return ReadBits( &outData, inBitCount ); }
	bool		Read( int& outData, uint32_t inBitCount = 32 )			{ return ReadBits( &outData, inBitCount ); }
	bool		Read( float& outData )									{ return ReadBits( &outData, 32 ); }

	bool		Read( uint16_t& outData, uint32_t inBitCount = 16 )		{ return ReadBits( &outData, inBitCount ); }
	bool		Read( int16_t& outData, uint32_t inBitCount = 16 )		{ return ReadBits( &outData, inBitCount ); }

	bool		Read( uint8_t& outData, uint32_t inBitCount = 8 )		{ return ReadBits( &outData, inBitCount ); }
	bool		Read( bool& outData )									{ return ReadBits( &outData, 1 ); }

	bool		Read( wstring& outData );

	void		ResetToCapacity( uint32_t inByteCapacity )				{ mBitCapacity = inByteCapacity << 3; mBitHead = 0; }

	friend void swap( IncomingPacketBuffer& inA, IncomingPacketBuffer& inB )
	{
		using std::swap;
		swap( inA.mData, inB.mData );
		swap( inA.mBitCapacity, inB.mBitCapacity );
		swap( inA.mBitHead, inB.mBitHead );
	}
};

