
class DeliveryNotificationManager
{
public:

	//in case we decide to change the type of the sequence number, to use fewer bits...
	typedef uint16_t	SequenceNumber;

	DeliveryNotificationManager( bool inShouldSendAcks, bool inShouldProcessAcks );
	~DeliveryNotificationManager();

	class TransmissionData
	{
	public:
		virtual void HandleDeliveryFailure( DeliveryNotificationManager* inDeliveryNotificationManager ) const = 0;
		virtual void HandleDeliverySuccess( DeliveryNotificationManager* inDeliveryNotificationManager ) const = 0;
	};
	typedef shared_ptr< TransmissionData > TransmissionDataPtr;

	class InFlightPacket
	{
	public:

		InFlightPacket( SequenceNumber inSequenceNumber );

		SequenceNumber	GetSequenceNumber() const	{ return mSequenceNumber; }
		float			GetTimeDispatched() const	{ return mTimeDispatched; }

		void			SetReplicationTransmissionData( shared_ptr< TransmissionData >	inTransmissionData ) { mReplicationTransmissionData = inTransmissionData; }
		TransmissionDataPtr GetReplicationTransmissionData() const { return mReplicationTransmissionData; }

		void			HandleDeliveryFailure( DeliveryNotificationManager* inDeliveryNotificationManager ) const;
		void			HandleDeliverySuccess( DeliveryNotificationManager* inDeliveryNotificationManager ) const;

	private:
		SequenceNumber	mSequenceNumber;
		float			mTimeDispatched;

		//this should really be a vector so we can have different kinds that we don't have to know about
		//but for now, this is simpler
		TransmissionDataPtr	mReplicationTransmissionData;
	};

	inline	InFlightPacket*		WriteState( OutgoingPacketBuffer& inPacketBuffer );
	inline	bool				ReadAndProcessState( IncomingPacketBuffer& inPacketBuffer );

			void				ProcessTimedOutPackets();

			uint32_t			GetDroppedPacketCount()		const	{ return mDroppedPacketCount; }
			uint32_t			GetDeliveredPacketCount()	const	{ return mDeliveredPacketCount; }
			uint32_t			GetDispatchedPacketCount()	const	{ return mDispatchedPacketCount; }

	const deque< InFlightPacket >&	GetInFlightPackets()	const	{ return mInFlightPackets; }

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

		void Write( OutgoingPacketBuffer& inPacketBuffer ) const;
		void Read( IncomingPacketBuffer& inPacketBuffer );

	private:
		SequenceNumber	mStart;
		uint32_t		mCount;
	};

	InFlightPacket*		WriteSequenceNumber( OutgoingPacketBuffer& inPacketBuffer );
	void				WriteAckData( OutgoingPacketBuffer& inPacketBuffer );

	//returns wether to drop the packet- if sequence number is too low!
	bool				ProcessSequenceNumber( IncomingPacketBuffer& inPacketBuffer );
	void				ProcessAcks( IncomingPacketBuffer& inPacketBuffer );


	void				AddPendingAck( SequenceNumber inSequenceNumber );
	void				HandlePacketDeliveryFailure( const InFlightPacket& inFlightPacket );
	void				HandlePacketDeliverySuccess( const InFlightPacket& inFlightPacket );

	SequenceNumber	mNextOutgoingSequenceNumber;
	SequenceNumber	mNextExpectedSequenceNumber;

	deque< InFlightPacket >	mInFlightPackets;
	deque< AckRange >		mPendingAcks;

	bool					mShouldSendAcks;
	bool					mShouldProcessAcks;

	uint32_t		mDeliveredPacketCount;
	uint32_t		mDroppedPacketCount;
	uint32_t		mDispatchedPacketCount;

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

inline DeliveryNotificationManager::InFlightPacket* DeliveryNotificationManager::WriteState( OutgoingPacketBuffer& inPacketBuffer )
{
	InFlightPacket* toRet = WriteSequenceNumber( inPacketBuffer );
	if( mShouldSendAcks )
	{
		WriteAckData( inPacketBuffer );
	}
	return toRet;
}

inline bool	DeliveryNotificationManager::ReadAndProcessState( IncomingPacketBuffer& inPacketBuffer )
{
	bool toRet = ProcessSequenceNumber( inPacketBuffer );
	if( mShouldProcessAcks )
	{
		ProcessAcks( inPacketBuffer );
	}
	return toRet;
}