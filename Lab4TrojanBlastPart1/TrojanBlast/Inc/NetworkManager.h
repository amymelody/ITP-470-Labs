class NetworkManager
{
public:
	static const uint32_t	kHelloCC = 'HELO';
	static const uint32_t	kWelcomeCC = 'WLCM';
	static const uint32_t	kStateCC = 'STAT';
	static const uint32_t	kInputCC = 'INPT';
	static const int		kMaxPacketsPerFrameCount = 10;

	NetworkManager();
	virtual ~NetworkManager();

	bool	Init( uint16_t inPort );
	void	ProcessIncomingPackets();

	virtual void	ProcessPacket( MemoryInputStream& inMemoryStream, const SocketAddress& inFromAddress ) = 0;
	virtual void	HandleConnectionReset( const SocketAddress& inFromAddress ) { ( void ) inFromAddress; }

			void	SendPacket( const MemoryOutputStream& inMemoryStream, const SocketAddress& inFromAddress );

			const WeightedTimedMovingAverage& GetBytesReceivedPerSecond()	const	{ return mBytesReceivedPerSecond; }
			const WeightedTimedMovingAverage& GetBytesSentPerSecond()		const	{ return mBytesSentPerSecond; }

			void	SetDropPacketChance( float inChance )	{ mDropPacketChance = inChance; }
			void	SetSimulatedLatency( float inLatency )	{ mSimulatedLatency = inLatency; }
private:

		class ReceivedPacket
		{
		public:
			ReceivedPacket( float inReceivedTime, MemoryInputStream& inMemoryInputStream, const SocketAddress& inAddress );

			const	SocketAddress&			GetFromAddress()	const	{ return mFromAddress; }
					float					GetReceivedTime()	const	{ return mReceivedTime; }
					MemoryInputStream&	GetMemoryStream()			{ return mMemoryStream; }

		private:
			
			float					mReceivedTime;
			MemoryInputStream	mMemoryStream;
			SocketAddress			mFromAddress;

		};

			void	UpdateBytesSentLastFrame();
			void	ReadIncomingPacketsIntoQueue();
			void	ProcessQueuedPackets();

	queue< ReceivedPacket, list< ReceivedPacket > >	mPacketQueue;

	UDPSocketPtr	mSocket;

	WeightedTimedMovingAverage	mBytesReceivedPerSecond;
	WeightedTimedMovingAverage	mBytesSentPerSecond;

	int							mBytesSentThisFrame;

	float						mDropPacketChance;
	float						mSimulatedLatency;
	
};

