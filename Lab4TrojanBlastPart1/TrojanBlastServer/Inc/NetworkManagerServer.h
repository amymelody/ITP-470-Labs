class NetworkManagerServer : public NetworkManager
{
public:
	static NetworkManagerServer*	sInstance;

	static bool				StaticInit( uint16_t inPort );
		
	virtual void			ProcessPacket( MemoryInputStream& inMemoryStream, const SocketAddress& inFromAddress ) override;
	virtual void			HandleConnectionReset( const SocketAddress& inFromAddress ) override;
		
			void			SendOutgoingPackets();
			void			CheckForDisconnects();

			int				GetNewNetworkId();

			ClientProxyPtr	GetClientProxy( int inPlayerId ) const;

private:
			NetworkManagerServer();

			void	HandlePacketFromNewClient( MemoryInputStream& inMemoryStream, const SocketAddress& inFromAddress );
			void	ProcessPacket( ClientProxyPtr inClientProxy, MemoryInputStream& inMemoryStream );
			
			void	SendWelcomePacket( ClientProxyPtr inClientProxy );
			void	UpdateAllClients();
			
			void	AddWorldStateToPacket( MemoryOutputStream& inStream );
			void	AddScoreBoardStateToPacket( MemoryOutputStream& inStream );

			void	SendStatePacketToClient( ClientProxyPtr inClientProxy );

			void	HandleInputPacket( ClientProxyPtr inClientProxy, MemoryInputStream& inMemoryStream );

			void	HandleClientDisconnected( ClientProxyPtr inClientProxy );

	typedef unordered_map< int, ClientProxyPtr >	IntToClientMap;
	typedef unordered_map< SocketAddress, ClientProxyPtr >	AddressToClientMap;

	AddressToClientMap		mAddressToClientMap;
	IntToClientMap			mPlayerIdToClientMap;

	int				mNewPlayerId;
	int				mNewNetworkId;

	float			mTimeOfLastSatePacket;
	float			mTimeBetweenStatePackets;
	float			mClientDisconnectTimeout;
};