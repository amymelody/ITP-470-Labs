class NetworkManagerServer : public NetworkManager
{
public:
	static NetworkManagerServer*	sInstance;

	static bool				StaticInit( uint16_t inPort );
		
	virtual void			ProcessPacket( IncomingPacketBuffer& inPacketBuffer, const SocketAddress& inFromAddress ) override;
	virtual void			HandleConnectionReset( const SocketAddress& inFromAddress ) override;
		
			void			SendOutgoingPackets();
			void			CheckForDisconnects();

			void			RegisterGameObject( GameObjectPtr inGameObject );
	inline	GameObjectPtr	RegisterAndReturn( GameObject* inGameObject );
			void			UnregisterGameObject( GameObject* inGameObject );
			void			SetStateDirty( int inNetworkId, uint32_t inDirtyState );

			ClientProxyPtr	GetClientProxy( int inPlayerId ) const;

private:
			NetworkManagerServer();

			void	HandlePacketFromNewClient( IncomingPacketBuffer& inPacketBuffer, const SocketAddress& inFromAddress );
			void	ProcessPacket( ClientProxyPtr inClientProxy, IncomingPacketBuffer& inPacketBuffer );
			
			void	SendWelcomePacket( ClientProxyPtr inClientProxy );
			void	UpdateAllClients();
			
			void	AddWorldStateToPacket( OutgoingPacketBuffer& inPacket );
			void	AddScoreBoardStateToPacket( OutgoingPacketBuffer& inPacket );

			void	SendStatePacketToClient( ClientProxyPtr inClientProxy );

			void	HandleInputPacket( ClientProxyPtr inClientProxy, IncomingPacketBuffer& inPacketBuffer );

			void	HandleClientDisconnected( ClientProxyPtr inClientProxy );

			int		GetNewNetworkId();

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


inline GameObjectPtr NetworkManagerServer::RegisterAndReturn( GameObject* inGameObject )
{
	GameObjectPtr toRet( inGameObject );
	RegisterGameObject( toRet );
	return toRet;
}
