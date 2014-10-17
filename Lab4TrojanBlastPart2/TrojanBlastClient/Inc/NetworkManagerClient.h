


class NetworkManagerClient : public NetworkManager
{
	enum NetworkClientState
	{
		NCS_Uninitialized,
		NCS_SayingHello,
		NCS_Welcomed
	};

public:
	static NetworkManagerClient*	sInstance;

	static	void	StaticInit( const SocketAddress& inServerAddress, const wstring& inName );

			void	SendOutgoingPackets();

	virtual void	ProcessPacket( IncomingPacketBuffer& inPacketBuffer, const SocketAddress& inFromAddress ) override;

private:
			NetworkManagerClient();
			void Init( const SocketAddress& inServerAddress, const wstring& inName );

			void	UpdateSayingHello();
			void	SendHelloPacket();

			void	HandleWelcomePacket( IncomingPacketBuffer& inPacketBuffer );
			void	HandleStatePacket( IncomingPacketBuffer& inPacketBuffer );

			void	HandleGameObjectState( IncomingPacketBuffer& inPacketBuffer );
			void	HandleScoreBoardState( IncomingPacketBuffer& inPacketBuffer );

			void	UpdateSendingInputPacket();
			void	SendInputPacket();

			void	DestroyGameObjectsInMap( const IntToGameObjectMap& inObjectsToDestroy );

				
	DeliveryNotificationManager	mDeliveryNotificationManager;
	ReplicationManagerClient	mReplicationManagerClient;

	SocketAddress		mServerAddress;

	NetworkClientState	mState;

	float				mTimeOfLastHello;
	float				mTimeOfLastInputPacket;

	wstring				mName;
	int					mPlayerId;

};