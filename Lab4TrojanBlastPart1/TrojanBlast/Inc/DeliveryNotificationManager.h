class DeliveryNotificationManager
{
public:

	//in case we decide to change the type of the sequence number, to use fewer bits...
	typedef uint16_t	SequenceNumber;

	DeliveryNotificationManager( bool inShouldSendAcks, bool inShouldProcessAcks );

	void LogStats( const wstring& inName );

	class InFlightPacket
	{
	public:

		InFlightPacket( SequenceNumber inSequenceNumber );

		SequenceNumber	GetSequenceNumber() const	{ return mSequenceNumber; }
		float			GetTimeDispatched() const	{ return mTimeDispatched; }

	private:
		SequenceNumber	mSequenceNumber;
		float			mTimeDispatched;

		//add transmission data about the packet here...
	};

	inline	InFlightPacket*		WriteState( MemoryOutputStream& inMemoryStream );
	inline	bool				ReadAndProcessState( MemoryInputStream& inMemoryStream );

			void				ProcessTimedOutPackets();

			uint32_t			GetDroppedPacketCount()		const	{ return mDroppedPacketCount; }
			uint32_t			GetDeliveredPacketCount()	const	{ return mDeliveredPacketCount; }
			uint32_t			GetDispatchedPacketCount()	const	{ return mDispatchedPacketCount; }

private:

	class AckRange
	{
	public:
		AckRange() : mStart( 0 ), mCount( 0 ) {}

		AckRange( SequenceNumber inStart ) : mStart( inStart ), mCount( 1 ) {}
		
		//if this is the next in sequence, just extend the range
		inline bool ExtendIfShould( SequenceNumber inSequenceNumber );

		SequenceNumber	GetStart() const { return mStart; } 
		uint32_t		GetCount() const { return mCount; } 

		void Write( MemoryOutputStream& inMemoryStream ) const;
		void Read( MemoryInputStream& inMemoryStream );

	private:
		SequenceNumber	mStart;
		uint32_t		mCount;
	};

	InFlightPacket*		WriteSequenceNumber( MemoryOutputStream& inMemoryStream );
	void				WriteAckData( MemoryOutputStream& inMemoryStream );

	//returns wether to drop the packet- if sequence number is too low!
	bool				ProcessSequenceNumber( MemoryInputStream& inMemoryStream );
	void				ProcessAcks( MemoryInputStream& inMemoryStream );


	void				AddPendingAck( SequenceNumber inSequenceNumber );
	void				HandlePacketDeliveryFailure( const InFlightPacket& inFlightPacket )	const;

	SequenceNumber	mNextOutgoingSequenceNumber;
	SequenceNumber	mNextExpectedSequenceNumber;

	deque< InFlightPacket >	mInFlightPackets;
	deque< AckRange >		mPendingAcks;

	bool					mShouldSendAcks;
	bool					mShouldProcessAcks;

	mutable uint32_t		mDeliveredPacketCount;
	mutable uint32_t		mDroppedPacketCount;
	mutable uint32_t		mDispatchedPacketCount;

};

inline bool DeliveryNotificationManager::AckRange::ExtendIfShould( SequenceNumber inSequenceNumber )
{
	if( inSequenceNumber == mStart + mCount )
	{
		++mCount;
		return true;
	}
	else
	{
		return false;
	}
}

inline DeliveryNotificationManager::InFlightPacket* DeliveryNotificationManager::WriteState( MemoryOutputStream& inMemoryStream )
{
	InFlightPacket* toRet = WriteSequenceNumber( inMemoryStream );
	if( mShouldSendAcks )
	{
		WriteAckData( inMemoryStream );
	}
	return toRet;
}

inline bool	DeliveryNotificationManager::ReadAndProcessState( MemoryInputStream& inMemoryStream )
{
	bool toRet = ProcessSequenceNumber( inMemoryStream );
	if( mShouldProcessAcks )
	{
		ProcessAcks( inMemoryStream );
	}
	return toRet;
}