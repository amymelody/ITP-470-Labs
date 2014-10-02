


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

	virtual void	ProcessPacket( MemoryInputStream& inMemoryStream, const SocketAddress& inFromAddress ) override;

private:
			NetworkManagerClient();
			void Init( const SocketAddress& inServerAddress, const wstring& inName );

			typedef unordered_map< int, GameObjectPtr > IntToGameObjectMap;

			void	UpdateSayingHello();
			void	SendHelloPacket();

			void	HandleWelcomePacket( MemoryInputStream& inMemoryStream );
			void	HandleStatePacket( MemoryInputStream& inMemoryStream );

			void	HandleGameObjectState( MemoryInputStream& inMemoryStream );
			void	HandleScoreBoardState( MemoryInputStream& inMemoryStream );

			void	AddToNetworkIdToGameObjectMap( GameObjectPtr inGameObject );
			void	RemoveFromNetworkIdToGameObjectMap( GameObjectPtr inGameObject );

			void	UpdateSendingInputPacket();
			void	SendInputPacket();

			void	DestroyGameObjectsInMap( const IntToGameObjectMap& inObjectsToDestroy );


	DeliveryNotificationManager	mDeliveryNotificationManager;

	SocketAddress		mServerAddress;

	NetworkClientState	mState;

	float				mTimeOfLastHello;
	float				mTimeOfLastInputPacket;

	wstring				mName;
	int					mPlayerId;

	IntToGameObjectMap	mNetworkIdToGameObjectMap;

};